#include "task.hpp"

Task::Task(
    const Appliance& appliance,
    TaskState state,
    int priority
)
    : appliance_(appliance),
      state_(state),
      priority_(priority),
      time_remaining_(appliance.cycleTime()),
      can_preempt_(appliance.interruptable()),
      id_(next_id_.fetch_add(1))
{}

void Task::runFor(int seconds) {
    time_remaining_ -= seconds;
}

const Appliance& Task::appliance() const { return appliance_; }
TaskState Task::state() const { return state_; }
int Task::priority() const { return priority_; }
int Task::arrivalTime() const { return arrival_time_; }
int Task::timeRemaining() const { return priority_; }
bool Task::canPreempt() const { return can_preempt_; }
int Task::id() const { return id_; }

void Task::setAppliance(const Appliance& appliance) { appliance_ = appliance; }
void Task::setState(TaskState state) { state_ = state; }
void Task::setPriority(int priority) { priority_ = priority; }
void Task::setTimeRemaining(int time_remaining) { time_remaining_ = time_remaining; }
void Task::setArrivalTime(int arrival_time) { arrival_time_ = arrival_time; }
void Task::setCanPreempt(bool can_preempt) { can_preempt_ = can_preempt; }
void Task::setId(int id) { id_ = id; }

std::atomic<int> Task::next_id_{0};