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
      running_task_(false),
      current_task_(nullptr),
      next_random_task_index_(0),
      simulation_clock_(0.0), // clock starts at midnight
      end_time_(60.0 * 24.0),
      time_step_(time_step)
{
    scheduler_thread_ = std::thread(&Scheduler::schedulerThread, this); // this refers to the object the function runs on
    runner_thread_ = std::thread(&Scheduler::taskRunnerThread, this);
}

Scheduler::~Scheduler() {
    if (scheduler_thread_.joinable())
        scheduler_thread_.join();

    if (runner_thread_.joinable())
        runner_thread_.join();
}

void Scheduler::start() {
    scheduler_thread_ = std::thread(&Scheduler::schedulerThread, this);
    runner_thread_ = std::thread(&Scheduler::taskRunnerThread, this);    
}

void Scheduler::wait() {

}

bool Scheduler::clockRunning() const { return simulation_clock_ < end_time_; }

void Scheduler::advanceClock() { simulation_clock_ += time_step_; }

void Scheduler::schedulerThread() {

    while (clockRunning()) { // each iteration represents a minute of simulation time
        std::this_thread::sleep_for(std::chrono::seconds(1));    
    
        { // start critical section
            std::lock_guard<std::mutex> lock(queue_mutex_);

            // advance simulated time
            simulation_clock_ += time_step_;

            // check predetermined tasks (simulation)
            while (next_random_task_index_ < random_tasks_.size()) {
                Task* task = random_tasks_[next_random_task_index_];

                // move task into queue if its time has come
                if (task->arrivalTime() == simulation_clock_) {
                    task_queue_.push(task);
                    ++next_random_task_index_;
                } else {
                    break;
                }
            }  
        } // end critical section

        printState();
        scheduler_cv_.notify_one(); // wake up scheduler if new task(s) have arrived
    }
}

void Scheduler::taskRunnerThread() {

    while (clockRunning()) {
        Task* task_to_run = nullptr;
            
        { // start critical section
            std::unique_lock<std::mutex> lock(queue_mutex_);

            // wait until there is at least one task in the queue or shutdown
            scheduler_cv_.wait(lock, [this] { return !task_queue_.empty() || !clockRunning(); });

            if (!clockRunning()) return;

            if (running_task_) { // check for preemption or keep running as normal
                if (!tryPreempt()) {
                    task_to_run = current_task_;
                }
            } else { // select new task for running
                current_task_ = task_queue_.top();
                task_queue_.pop();

                running_task_ = true;
                task_to_run = current_task_;
            }
        } // end critical section

        // run task outside the critical section
        if (task_to_run) {
            runCurrentTask();
        }
    }
}

void Scheduler::runCurrentTask() {
    current_task_->runFor(time_step_);

    // deplete greywater store if the appliance can use it
    if ( current_task_->appliance().takesGreywater() &&
        water_system_.currentGreywaterStore() >= current_task_->appliance().totalWaterUsage())
    {
        water_system_.updateCurrentGreywaterStore(current_task_->appliance().waterUsagePerMinute());
    }

    // replenish greywater store if appliance creates it
    if (current_task_->appliance().producesGreywater()) {
        water_system_.updateCurrentGreywaterStore(current_task_->appliance().waterOutputPerCycle());
    }

    // allow for retrieval of a new task when current task finished
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);

        if (current_task_->finished()) {
            running_task_ = false; // shared resource
            current_task_ = nullptr;
        }
    }

    scheduler_cv_.notify_one(); // wake runner if task finished
}

bool Scheduler::tryPreempt() {
    if (!task_queue_.empty()) { // if there's a higher priotity task ready in the queue

        if (current_task_->canPreempt() &&
            task_queue_.top()->priority() > current_task_->priority() &&
            task_queue_.top()->status() == READY)
        {
            updatePriority(current_task_);
            task_queue_.push(current_task_);

            current_task_ = task_queue_.top();
            task_queue_.pop();

            return true;
        }
    }
    return false;
}

void Scheduler::updatePriority(Task* task) {
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
