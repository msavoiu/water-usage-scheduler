#include <nlohmann/json.hpp>
#include <fstream>

#include "household.hpp"

Household::Household(const std::string& config_file) {
    std::ifstream file(config_file);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open config file: " + config_file);
    }

    nlohmann::json data;
    file >> data;

    for (const auto& item : data) {
        if (item["type"] == "Greywater") {
            appliances_.[item["name"]] =
                std::make_unique<GreywaterAppliance>(
                    item["name"],
                    item["cycle_time"],
                    item["water_usage_per_minute"],
                    item["total_water_usage"],
                    item["interruptable"],
                    item["takes_greywater"],
                    item["water_output_per_cycle"]
                );

        } else {
            appliances_[item["name"]] =
                std::make_unique<Appliance>(
                    item["name"],
                    item["cycle_time"],
                    item["water_usage_per_minute"],
                    item["total_water_usage"],
                    item["interruptable"],
                    item["takes_greywater"]
                );
        }
    }
}

const std::vector<std::unique_ptr<Appliance>>& Household::getAppliances() const { return appliances_; }
