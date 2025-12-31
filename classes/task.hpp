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
            int priority
        );

        void runFor(int seconds);

        // Getters
        const Appliance& appliance() const;
        TaskState state() const;
        int priority() const;
        int timeRemaining() const;
        int arrivalTime() const;
        bool canPreempt() const;
        int id() const;

        // Setters
        void setAppliance(const Appliance& appliance);
        void setState(TaskState state);
        void setPriority(int priority);
        void setTimeRemaining(int time_remaining);
        void setArrivalTime(int arrival_time);
        void setCanPreempt(bool can_preempt);
        void setId(int id);


    private:
        const Appliance& appliance_;

        TaskState state_;
        int priority_;
        int arrival_time_; // minutes since midnight
        int time_remaining_;
        bool can_preempt_;

        int id_;
        static std::atomic<int> next_id_;
};