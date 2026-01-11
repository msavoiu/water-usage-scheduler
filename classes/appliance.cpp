#include "appliance.hpp"

Appliance::Appliance(
    const std::string& name,
    double cycle_time,
    double water_usage_per_minute,
    double total_water_usage,
    bool interruptable,
    bool takes_greywater
)
    : name_(name),
      cycle_time_(cycle_time),
      water_usage_per_minute_(water_usage_per_minute),
      total_water_usage_(total_water_usage),
      interruptable_(interruptable),
      takes_greywater_(takes_greywater)
{
    produces_greywater_ = false;
}

Appliance::Appliance(
    const Appliance& other
) 
    : name_(other.name()),
    cycle_time_(other.cycleTime()),
    water_usage_per_minute_(other.waterUsagePerMinute()),
    total_water_usage_(other.totalWaterUsage()),
    interruptable_(other.interruptable()),
    takes_greywater_(other.takesGreywater())
{}

// Setters
void Appliance::setName(const std::string& name) { name_ = name; }
void Appliance::setCycleTime(int minutes) { cycle_time_ = minutes; }
void Appliance::setWaterUsagePerMinute(double liters) { water_usage_per_minute_ = liters; }
void Appliance::setTotalWaterUsage(double total_water_usage) { total_water_usage_ = total_water_usage; }
void Appliance::setInterruptable(bool interruptable) { interruptable_ = interruptable; }
void Appliance::setTakesGreywater(bool takes_greywater) { takes_greywater_ = takes_greywater; }
void Appliance::setProducesGreywater(bool produces_greywater) { produces_greywater_ = produces_greywater; }

// Getters
std::string Appliance::name() const { return name_; }
int Appliance::cycleTime() const { return cycle_time_; }
double Appliance::waterUsagePerMinute() const { return water_usage_per_minute_; }
double Appliance::totalWaterUsage() const { return total_water_usage_; };
bool Appliance::interruptable() const { return interruptable_; }
bool Appliance::takesGreywater() const { return takes_greywater_; }
bool Appliance::producesGreywater() const { return produces_greywater_; }


GreywaterAppliance::GreywaterAppliance(
    const std::string& name,
    int cycle_time,
    double water_usage_per_minute,
    double total_water_usage,
    bool interruptable,
    bool takes_greywater,
    double water_output_per_cycle
)
    : Appliance(
        name,
        cycle_time,
        water_usage_per_minute,
        total_water_usage,
        interruptable,
        takes_greywater
    ),
    water_output_per_cycle_(water_output_per_cycle)
{
    produces_greywater_ = true;
}

// Setters
void GreywaterAppliance::setWaterOutputPerCycle(double water_output_per_cycle) { water_output_per_cycle_ = water_output_per_cycle; }

// Getters
double GreywaterAppliance::waterOutputPerCycle() const { return water_output_per_cycle_; }  
