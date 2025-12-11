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

std::atomic<int> Task::next_id_{0};