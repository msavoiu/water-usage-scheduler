#include <iostream>
#include <chrono>
#include <thread>

#include "classes/appliance.hpp"
#include "classes/task.hpp"
#include "classes/water_system.hpp"
#include "classes/household.hpp"
#include "classes/scheduler.hpp"

int main() {
    WaterSystem water_system(10.0, 5.0, 0.0);
    std::vector<std::unique_ptr<Task>> predefined_tasks;

    Scheduler scheduler(
        predefined_tasks,
        water_system,
        60.0 // time_step
    );

    scheduler.start(); // starts threads
    scheduler.wait(); // joins threads

    std::cout << "Scheduling complete\n";

    // while (scheduler.clockRunning()) {
    //     scheduler.schedulerThread();
    //     scheduler.taskRunnerThread();

    //     // update CLI display
    //     scheduler.printState();

    //     // sleep for 1 second between each simulated minute of wall-clock time
    //     scheduler.advanceClock();
    //     std::this_thread::sleep_for(
    //         std::chrono::duration<double>(1.0)
    //     );
    // }

    std::cout << "Scheduling complete\n";

    return 0;
}