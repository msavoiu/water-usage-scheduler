#include "classes/appliance.hpp"
#include "classes/task.hpp"
#include "classes/water_system.hpp"
#include "classes/household.hpp"
#include "classes/scheduler.hpp"

int main() {
    WaterSystem water_system(10.0, 5.0, 0.0);
    std::vector<std::unique_ptr<Task>> predefined_tasks;

    Scheduler scheduler(predefined_tasks, water_system);

    while (scheduler.clockRunning()) {
        scheduler.arrivalThread();
        scheduler.schedulerThread();

        scheduler.advanceClock();
    }

    return 0;
}