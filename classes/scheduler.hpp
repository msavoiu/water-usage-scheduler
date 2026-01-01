#include <vector>
#include <memory>
#include <queue>
#include <mutex>

#include "task.hpp"
#include "water_system.hpp"

class Scheduler {
    public:
        Scheduler(
            std::vector<std::unique_ptr<Task>> predefined_tasks,
            WaterSystem& water_system,
            double time_step
        );

        // Handling simulation time
        bool clockRunning() const;
        void advanceClock();

        void arrivalThread();
        void schedulerThread();

        void updatePriority(std::unique_ptr<Task> task);

        // TERMINAL DISPLAY
        void printState() const;

    private:
        WaterSystem& water_system_;

        std::vector<std::unique_ptr<Task>> all_tasks_;

        std::priority_queue<
            Task*,
            std::vector<Task*>
        > task_queue_;

        std::mutex queue_mutex_;
        std::condition_variable queue_cv_;

        double simulation_clock_;
        double end_time_;
        double time_step_;
};
