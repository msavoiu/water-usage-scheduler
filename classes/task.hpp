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
            int priority,
        );

    private:
        Appliance appliance;

        TaskState state;
        int priority;
        double time_remaining;
        bool can_preempt;

        int id;
        static std::atomic<int> next_id;
};