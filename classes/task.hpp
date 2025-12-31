#include <atomic>

#include "appliance.hpp"

enum TaskState {
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
            TaskState state,
            int base_priority
        );

        void runFor(int seconds);

        // Getters
        const Appliance& appliance() const;
        TaskState state() const;
        int basePriority() const;
        int priority() const;
        double timeRemaining() const;
        double arrivalTime() const;
        bool canPreempt() const;
        int preemptions() const;
        int id() const;

        // Setters
        void setAppliance(const Appliance& appliance);
        void setState(TaskState state);
        void setBasePriority(int base_priority);
        void setPriority(int priority);
        void setTimeRemaining(double time_remaining);
        void setArrivalTime(double arrival_time);
        void setCanPreempt(bool can_preempt);
        void setPreemptions(bool preemptions);
        void setId(int id);

        bool operator<(const Task& other) const; 

    private:
        const Appliance& appliance_;

        TaskState state_;
        int base_priority_;
        int priority_;
        double arrival_time_; // minutes since midnight
        double time_remaining_;
        bool can_preempt_;
        int preemptions_;

        int id_;
        static std::atomic<int> next_id_;
};