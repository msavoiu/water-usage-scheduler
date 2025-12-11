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
            appliances.emplace_back(
                item["name"],
                item["cycle_time"],
                item["water_usage_per_minute"].get<std::vector<double>>(),
                item["interruptable"],
                item["takes_greywater"],
                item["water_output_per_cycle"]
            );

        } else {
            appliances_.emplace_back(
                item["name"],
                item["cycle_time"],
                item["water_usage_per_minute"].get<std::vector<double>>(),
                item["interruptable"],
                item["takes_greywater"]
            );
        }
    }
}

std::vector<std::unique_ptr<Appliance>> Household::getAppliances() const { return appliances_; }
