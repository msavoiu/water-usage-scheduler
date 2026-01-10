#include <vector>
#include <memory>
#include <queue>
#include <mutex>
#include <thread>

#include "task.hpp"
#include "water_system.hpp"

class Scheduler {
    public:
        Scheduler(
            std::vector<std::unique_ptr<Task>> predefined_tasks,
            WaterSystem& water_system,
            double time_step
        );

        ~Scheduler();

        // SIMULATION
        void start();
        void wait();
        bool clockRunning() const;
        void advanceClock();

        void schedulerThread(); // asimulates arrival of new tasks from appliances, adding new arrivals to the queue
        void taskRunnerThread(); // facilitate running/preempting of tasks

        bool tryPreempt();
        void runCurrentTask();
        void updatePriority(Task* task);

        void printState();

    private:
        WaterSystem& water_system_;

        std::vector<std::unique_ptr<Task>> all_tasks_;

        std::priority_queue<
            Task*,
            std::vector<Task*>,
            TaskCompare
        > task_queue_;

        bool running_task_;
        Task* current_task_;

        // THREADING
        std::thread scheduler_thread_;
        std::thread runner_thread_;
        std::mutex queue_mutex_;
        std::condition_variable scheduler_cv_;

        // parsed from a JSON file for simulating arrival of random tasks throughout a day in a household
        std::vector<Task*> random_tasks_;
        size_t next_random_task_index_;

        double simulation_clock_;
        double end_time_;
        double time_step_;
};