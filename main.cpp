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

    Scheduler scheduler(
        water_system,
        60.0 // time_step
    );

    scheduler.start();

    scheduler.wait(); // joins threads

    std::cout << "Scheduling complete\n";

    return 0;
}