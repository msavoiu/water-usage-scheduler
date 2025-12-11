#include "water_system.hpp"

WaterSystem::WaterSystem(
            double water_supply_per_hour,
            double greywater_storage_capacity,
            double current_greywater_store
)
    : water_supply_per_hour_(water_supply_per_hour),
      greywater_storage_capacity_(greywater_storage_capacity),
      current_greywater_store_(current_greywater_store)
{}

// Setters
void WaterSystem::setWaterSupplyPerHour(double water_supply_per_hour) { water_supply_per_hour_ = water_supply_per_hour; }
void WaterSystem::setGreywaterStorageCapacity(double greywater_storage_capacity) { greywater_storage_capacity_ = greywater_storage_capacity; }
void WaterSystem::setCurrentGreywaterStore(double current_greywater_store) { current_greywater_store_ = current_greywater_store; }

// Setters
double WaterSystem::waterSupplyPerHour() const { return water_supply_per_hour_; }
double WaterSystem::greywaterStorageCapacity() const { return greywater_storage_capacity_; }
double WaterSystem::currentGreywaterStore() const { return current_greywater_store_; }