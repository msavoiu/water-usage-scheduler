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
        const std::string name = item["name"];

        if (item["type"] == "Greywater") {
            appliances_.emplace(
                name,
                std::make_unique<GreywaterAppliance>(
                    item["name"].get<std::string>(),
                    item["cycle_time"].get<double>(),
                    item["water_usage_per_minute"].get<double>(),
                    item["interruptable"].get<bool>(),
                    item["takes_greywater"].get<bool>(),
                    item["water_output_per_cycle"].get<double>()
                )
            );

        } else {
            appliances_.emplace(
                name,
                std::make_unique<Appliance>(
                    item["name"].get<std::string>(),
                    item["cycle_time"].get<double>(),
                    item["water_usage_per_minute"].get<double>(),
                    item["interruptable"].get<bool>(),
                    item["takes_greywater"].get<bool>()
                )
            );
        }
    }
}

const std::unordered_map<std::string, std::unique_ptr<Appliance>>& Household::appliances() const { return appliances_; }
