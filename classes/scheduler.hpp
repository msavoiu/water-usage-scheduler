#include <vector>
#include <memory>

#include "task.hpp"
#include "water_system.hpp"

class Scheduler {
    public:
        Scheduler(std::vector<std::unique_ptr<Task>>& tasks, WaterSystem& ws);

        void simulateDay();
        void scheduleTasks();

    private:
        std::vector<std::unique_ptr<Task>>& tasks;
        WaterSystem& water_system;
};
