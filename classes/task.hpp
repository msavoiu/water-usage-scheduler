#include <atomic>
#include <sstream>

#include "appliance.hpp"

enum TaskStatus {
    NEW,
    RUNNING,
    WAITING,
    READY,
    TERMINATED
};

class Task {
    public:
        Task(
            const Appliance& appliance,
            TaskStatus status_,
            int base_priority
        );

        bool finished(); 
        void runFor(int seconds);

        // Getters
        const Appliance& appliance() const;
        TaskStatus status() const;
        int basePriority() const;
        int priority() const;
        double timeRemaining() const;
        double arrivalTime() const;
        bool canPreempt() const;
        int preemptions() const;
        int id() const;

        // Setters
        void setAppliance(const Appliance& appliance);
        void setStatus(TaskStatus status);
        void setBasePriority(int base_priority);
        void setPriority(int priority);
        void setTimeRemaining(double time_remaining);
        void setArrivalTime(double arrival_time);
        void setCanPreempt(bool can_preempt);
        void setPreemptions(bool preemptions);
        void setId(int id);

        bool operator>(const Task& other) const; 

    private:
        const Appliance& appliance_;

        TaskStatus status_;
        int base_priority_;
        int priority_;
        double arrival_time_; // minutes since midnight
        double time_remaining_;
        bool can_preempt_;
        int preemptions_;

        int id_;
        static std::atomic<int> next_id_;
};

// HELPERS

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

struct TaskCompare {
    bool operator()(const std::unique_ptr<Task>& a,
                    const std::unique_ptr<Task>& b) const {
        // status first
        if (statusRank(a->status()) != statusRank(b->status()))
            return statusRank(a->status()) > statusRank(b->status());

        // then priority (descending)
        return a->priority() < b->priority();
    }
};

bool taskPrintCompare(const Task* a, const Task* b) {
    int ra = statusRank(a->status());
    int rb = statusRank(b->status());

    if (ra != rb)
        return ra < rb;

    return a->priority() > b->priority();
}
