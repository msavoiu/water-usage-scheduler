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

const Appliance& Task::appliance() const { return appliance_; }
TaskState Task::state() const { return state_; }
int Task::priority() const { return priority_; }
double Task::timeRemaining() const { return priority_; }
bool Task::canPreempt() const { return can_preempt_; }
int Task::id() const { return id_; }

std::atomic<int> Task::next_id_{0};