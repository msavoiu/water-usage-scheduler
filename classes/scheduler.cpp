#include <utility>
#include <thread>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>

#include "scheduler.hpp"
#include "string"

Scheduler::Scheduler(
    std::vector<std::unique_ptr<Task>> predefined_tasks,
    WaterSystem& water_system,
    double time_step
)
    : predefined_tasks_(predefined_tasks),
      water_system_(water_system),
      simulation_clock_(0.0), // clock starts at midnight
      end_time_(1440.0),
      time_step_(time_step)
{} 

bool Scheduler::clockRunning() const { return simulation_clock_ < end_time_; }

void Scheduler::advanceClock() { simulation_clock_ += time_step_; }

void Scheduler::arrivalThread() {
    size_t next_task_index = 0;

    while (clockRunning()) { // each iteration represents a minute of simulation time
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
    while (clockRunning()) {
        std::unique_ptr<Task> curr_task;

        { // start critical section
            std::unique_lock<std::mutex> lock(queue_mutex_);

            // wait until there is a task or shutdown
            queue_cv_.wait(lock, [&] { return !task_queue_.empty() || !clockRunning(); });

            if (!clockRunning()) {
                break;
            }

            // pop ready task from queue
            curr_task = std::move(const_cast<std::unique_ptr<Task>&>(task_queue_.top()));
            task_queue_.pop();

        } // end critical section

        // run task
        while (curr_task->timeRemaining() > 0 && clockRunning()) {
            curr_task->runFor(1); // run task a second at a time
            simulation_clock_ -= 1.0;

            if (curr_task->canPreempt() && ) {
                { // start critical section
                    std::lock_guard<std::mutex> lock(queue_mutex_);

                    if (!task_queue_.empty() && task_queue_.top()->priority() > curr_task->priority()) {
                        // preempt current task, add it back to the queue
                        task_queue_.push(std::move(curr_task));
                        break;
                    }
                }
            }
        }
    }
}

void Scheduler::updatePriority(std::unique_ptr<Task> task) {
    int priority = task->priority();

    // INCREASE
    priority += task->preemptions();
    if (task->appliance().producesGreywater()) {
        // increase priority
    }

    // DECREASE
    priority -= task->timeRemaining();

    // GREYWATER CONSIDERATIONS
    if (task->appliance().takesGreywater() &&
        water_system_.currentGreywaterStore() < task->appliance().totalWaterUsage()) {
        // decrease priority
    } else if (task->appliance().takesGreywater()) {
        // increase priority (since greywater is available to use right now)
    }

    task->setPriority(priority);
}
