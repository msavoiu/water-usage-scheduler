#include <sstream>
#include <iomanip>

#include "task.hpp"

Task::Task(
    Appliance& appliance,
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

bool Task::finished() {
    return (status_ == TERMINATED);
}

void Task::runFor(int seconds) {
    if (time_remaining_ > 0.0) {
        time_remaining_ -= seconds;
    } else {
        status_ = TERMINATED;
    }
}

Appliance& Task::appliance() const { return appliance_; }
TaskStatus Task::status() const { return status_; }
int Task::basePriority() const { return base_priority_; }
int Task::priority() const { return priority_; }
double Task::arrivalTime() const { return arrival_time_; }
double Task::timeRemaining() const { return priority_; }
bool Task::canPreempt() const { return can_preempt_; }
int Task::preemptions() const { return preemptions_; }
int Task::id() const { return id_; }

void Task::setAppliance(Appliance& appliance) { appliance_ = appliance; }
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

// HELPERS
bool TaskCompare::operator()(const Task* a, const Task* b) const {
    // status first
    if (statusRank(a->status()) != statusRank(b->status()))
        return statusRank(a->status()) > statusRank(b->status());

    // then priority (descending)
    return a->priority() < b->priority();
}

int statusRank(TaskStatus s) {
    switch (s) {
        case TaskStatus::RUNNING:    return 0;
        case TaskStatus::WAITING:    return 1;
        case TaskStatus::READY:      return 2;
        case TaskStatus::NEW:        return 3;
        case TaskStatus::TERMINATED: return 4;
    }
    return 5; // fallback
}

bool isActive(TaskStatus s) {
    return s == TaskStatus::RUNNING || s == TaskStatus::READY;
}

std::string statusToString(TaskStatus s) {
    switch (s) {
        case TaskStatus::RUNNING:    return "RUNNING";
        case TaskStatus::WAITING:    return "WAITING";
        case TaskStatus::READY:      return "READY";
        case TaskStatus::NEW:        return "NEW";
        case TaskStatus::TERMINATED: return "TERMINATED";
    }
    return "UNKNOWN";
}

std::string formatTime(double minutes) {
    int total = static_cast<int>(minutes);
    int h = total / 60;
    int m = total % 60;

    std::ostringstream oss;
    oss << h << "h "
        << std::setw(2) << std::setfill('0') << m << "m";
    return oss.str();
}

bool taskPrintCompare(const Task* a, const Task* b) {
    int ra = statusRank(a->status());
    int rb = statusRank(b->status());

    if (ra != rb)
        return ra < rb;

    return a->priority() > b->priority();
}
