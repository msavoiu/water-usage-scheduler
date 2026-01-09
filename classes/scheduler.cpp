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
    : all_tasks_(predefined_tasks),
      water_system_(water_system),
      next_random_task_index_(0),
      simulation_clock_(0.0), // clock starts at midnight
      end_time_(1440.0),
      time_step_(time_step)
{} 

bool Scheduler::clockRunning() const { return simulation_clock_ < end_time_; }

void Scheduler::advanceClock() { simulation_clock_ += time_step_; }

void Scheduler::arrivalThread() {
    // size_t next_task_index = 0;

    // while (clockRunning()) { // each iteration represents a minute of simulation time
    //     double current_time_ = simulation_clock_;

    //     { // start critical section
    //         std::lock_guard<std::mutex> lock(queue_mutex_);

    //         // check predetermined tasks (simulation)
    //         for (size_t i = next_task_index; i < all_tasks_.size(); ++i) {
    //             auto& task = all_tasks_[i];

    //             // move task into queue if its time has come
    //             if (task->arrivalTime() == current_time_) {
    //                 task_queue_.push(task.get()); // need to push raw pointer to the object managed by the smart pointers
    //             } else {
    //                 next_task_index = i;
    //                 break;
    //             }
    //         }
            
    //     } // end critical section

    // only add on tasks arriving at this particular tick in time
    while (random_tasks_[next_random_task_index_]->arrivalTime() == simulation_clock_) {
        { // critical section
            std::lock_guard<std::mutex> lock(queue_mutex_);
            task_queue_.push(random_tasks_[next_random_task_index_]);
            ++next_random_task_index_;
        }
    }

    queue_cv_.notify_one(); // wake up scheduler when new task(s) have arrived

}

void Scheduler::schedulerThread() {
    while (clockRunning()) {
        Task* curr_task;

        { // start critical section
            std::unique_lock<std::mutex> lock(queue_mutex_);

            // wait until there is a task or shutdown
            queue_cv_.wait(lock, [&] { return !task_queue_.empty() || !clockRunning(); });

            if (!clockRunning()) {
                break;
            }

            // pop ready task from queue
            curr_task = task_queue_.top();
            task_queue_.pop();

        } // end critical section

        // run task
        while (curr_task->timeRemaining() > 0 && clockRunning()) {
            curr_task->runFor(1); // run task a second at a time
            simulation_clock_ -= 1.0;

            if (curr_task->canPreempt() && task_queue_.top()->priority() < curr_task->priority()) { // there's a better priotity task ready
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
        // increase priority (since greywater is available to use right now
    }

    task->setPriority(priority);
}

void Scheduler::printState() {
    std::lock_guard<std::mutex> lock(queue_mutex_);

    // clear screen
    std::cout << "\033[2J\033[H";

    std::vector<const Task*> active;
    std::vector<const Task*> inactive;

    active.reserve(all_tasks_.size());
    inactive.reserve(all_tasks_.size());

    // Split tasks
    for (const auto& t : all_tasks_) {
        if (isActive(t->status()))
            active.push_back(t.get());
        else
            inactive.push_back(t.get());
    }

    // Sort for display
    std::sort(active.begin(), active.end(), taskPrintCompare);
    std::sort(inactive.begin(), inactive.end(), taskPrintCompare);

    auto printHeader = []() {
        std::cout << std::left
                  << std::setw(8)  << "ID" << "| "
                  << std::setw(20) << "Appliance" << "| "
                  << std::setw(13) << "Status" << "| "
                  << std::setw(12) << "Priority" << "| "
                  << "Time Remaining\n";

        std::cout
            << "--------+--------------------+---------------+-------------+-------------------\n";
    };

    // ---------- ACTIVE ----------
    std::cout << "ACTIVE TASKS:\n";
    printHeader();

    for (const Task* t : active) {
        std::cout << std::left
                  << std::setw(8)  << t->id() << "| "
                  << std::setw(20) << t->appliance().name() << "| "
                  << std::setw(13) << statusToString(t->status()) << "| "
                  << std::setw(12) << t->priority() << "| "
                  << formatTime(t->timeRemaining())
                  << '\n';
    }

    std::cout << "\nGREYWATER: "
              << water_system_.currentGreywaterStore() << "/"
              << water_system_.greywaterStorageCapacity()
              << " liters available\n\n";

    // ---------- INACTIVE ----------
    std::cout << "INACTIVE TASKS:\n";
    printHeader();

    for (const Task* t : inactive) {
        std::cout << std::left
                  << std::setw(8)  << t->id() << "| "
                  << std::setw(20) << t->appliance().name() << "| "
                  << std::setw(13) << statusToString(t->status()) << "| "
                  << std::setw(12) << t->priority() << "| "
                  << formatTime(t->timeRemaining())
                  << '\n';
    }

    std::cout << std::flush;
}

