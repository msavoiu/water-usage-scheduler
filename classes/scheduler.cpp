#include <utility>
#include <thread>

#include "scheduler.hpp"

Scheduler::Scheduler(
    std::vector<std::unique_ptr<Task>> predefined_tasks,
    WaterSystem& water_system
)
    : predefined_tasks_(predefined_tasks),
      water_system_(water_system),
      simulation_clock_(0.0), // clock starts at midnight
      end_time_(1440.0),
      time_step_(1.0) // 1 minute (1 second in simulation time)
{}

void Scheduler::arrivalThread() {
    size_t next_task_index = 0;

    while (running_) { // each iteration represents a minute of simulation time
        double current_time_ = simulation_clock_;

        { // start critical section
            std::lock_guard<std::mutex> lock(queue_mutex_);

            // check predetermined tasks (simulation)
            for (size_t i = next_task_index; i < predefined_tasks_.size(); ++i) {
                auto& task = predefined_tasks_[i];

                // move task into queue if its time has come
                if (task->arrivalTime() == current_time_) {
                    task_queue_.push(std::move(task));
                } else {
                    next_task_index = i;
                    break;
                }
            }
            
        } // end critical section

        queue_cv_.notify_one(); // wake up scheduler when new task(s) have arrived
    }
}

void Scheduler::schedulerThread() {
    while (running_) {
        std::unique_ptr<Task> next_task;

        { // start critical section
            std::unique_lock<std::mutex> lock(queue_mutex_);

            // wait until there is a task or shutdown
            queue_cv_.wait(lock, [&] { return !task_queue_.empty() || !running_; });

            if (!running_) {
                break;
            }

            // pop ready task from queue
            next_task = std::move(const_cast<std::unique_ptr<Task>&>(task_queue_.top()));
            task_queue_.pop();

        } // end critical section

        next_task->runFor(next_task->timeRemaining());
    }
}
