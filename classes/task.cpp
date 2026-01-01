#include "task.hpp"

Task::Task(
    const Appliance& appliance,
    TaskStatus status,
    int base_priority
)
    : appliance_(appliance),
      status_(status),
      base_priority_(base_priority),
      time_remaining_(appliance.cycleTime()),
      can_preempt_(appliance.interruptable()),
      id_(next_id_.fetch_add(1))
{}

void Task::runFor(int seconds) {
    time_remaining_ -= seconds;
}

const Appliance& Task::appliance() const { return appliance_; }
TaskStatus Task::status() const { return status_; }
int Task::basePriority() const { return base_priority_; }
int Task::priority() const { return priority_; }
double Task::arrivalTime() const { return arrival_time_; }
double Task::timeRemaining() const { return priority_; }
bool Task::canPreempt() const { return can_preempt_; }
int Task::preemptions() const { return preemptions_; }
int Task::id() const { return id_; }

void Task::setAppliance(const Appliance& appliance) { appliance_ = appliance; }
void Task::setStatus(TaskStatus status) { status_ = status; }
void Task::setBasePriority(int base_priority) { base_priority_ = base_priority; }
void Task::setPriority(int priority) { priority_ = priority; }
void Task::setTimeRemaining(double time_remaining) { time_remaining_ = time_remaining; }
void Task::setArrivalTime(double arrival_time) { arrival_time_ = arrival_time; }
void Task::setCanPreempt(bool can_preempt) { can_preempt_ = can_preempt; }
void Task::setPreemptions(bool preemptions) { preemptions_ = preemptions; }
void Task::setId(int id) { id_ = id; }

std::atomic<int> Task::next_id_{0};

// operator overload for priority queue
bool Task::operator>(const Task& other) const {
    return (priority_ > other.priority());
}
