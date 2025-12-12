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

        // Getters
        const Appliance& appliance() const;
        TaskState state() const;
        double timeRemaining() const;
        bool canPreempt() const;
        int id() const;

    private:
        const Appliance& appliance_;

        TaskState state_;
        int priority_;
        double time_remaining_;
        bool can_preempt_;

        int id_;
        static std::atomic<int> next_id_;
};