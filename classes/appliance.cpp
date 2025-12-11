#include "appliance.hpp"

Appliance::Appliance(
    const std::string& name,
    double cycle_time,
    const std::vector<double>& water_usage_per_minute,
    bool interruptable,
    bool takes_greywater
)
    : name_(name),
      cycle_time_(cycle_time),
      water_usage_per_minute_(water_usage_per_minute),
      interruptable_(interruptable),
      takes_greywater_(takes_greywater)
{}

// Setters
void Appliance::setName(const std::string& name) { name_ = name; }
void Appliance::setCycleTime(double minutes) { cycle_time_ = minutes; }
void Appliance::setWaterUsagePerMinute(const std::vector<double>& liters) { water_usage_per_minute_ = liters; }
void Appliance::setInterruptable(bool interruptable) { interruptable_ = interruptable; }
void Appliance::setTakesGreywater(bool takes_greywater) { takes_greywater_ = takes_greywater; }

// Getters
std::string Appliance::name() const { return name_; }
double Appliance::cycleTime() const { return cycle_time_; }
const std::vector<double>& Appliance::waterUsagePerMinute() const { return water_usage_per_minute_; }
bool Appliance::interruptable() const { return interruptable_; }
bool Appliance::takesGreywater() const { return takes_greywater_; }


GreywaterAppliance::GreywaterAppliance(
    const std::string& name,
    double cycle_time,
    const std::vector<double>& water_usage_per_minute,
    bool interruptable,
    double water_output_per_cycle
)
    : Appliance(
        name,
        cycle_time,
        water_usage_per_minute,
        interruptable,
        false
    ),
    water_output_per_cycle_(water_output_per_cycle)
{}

// Setters
void GreywaterAppliance::setWaterOutputPerCycle(double water_output_per_cycle) { water_output_per_cycle_ = water_output_per_cycle; }

// Getters
double GreywaterAppliance::waterOutputPerCycle() const { return water_output_per_cycle_; }  
