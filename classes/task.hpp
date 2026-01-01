#include <atomic>

#include "appliance.hpp"

class Task {
    public:
        Task(
            const Appliance& appliance,
            TaskStatus status_,
            int base_priority
        );

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

// STATUS AND HELPERS
enum TaskStatus {
    NEW,
    RUNNING,
    WAITING,
    READY,
    TERMINATED
};

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
