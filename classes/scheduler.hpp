#include <vector>
#include <memory>
#include <queue>
#include <mutex>

#include "task.hpp"
#include "water_system.hpp"

// For priority queue's use
struct TaskComparator {
    bool operator()(const std::unique_ptr<Task>& a,
                    const std::unique_ptr<Task>& b) const {
        return a->priority() < b->priority();
    }
};

class Scheduler {
    public:
        Scheduler(std::vector<std::unique_ptr<Task>> predefined_tasks, WaterSystem& water_system);

        void arrivalThread();
        void schedulerThread();

    private:
        std::vector<std::unique_ptr<Task>> predefined_tasks_;
        WaterSystem& water_system_;

        bool running_;

        std::priority_queue<
            std::unique_ptr<Task>,
            std::vector<std::unique_ptr<Task>>,
            TaskComparator
        > task_queue_;

        std::mutex queue_mutex_;
        std::condition_variable queue_cv_;

        double simulation_clock_;
        double end_time_;
        double time_step_;
};
