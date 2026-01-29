#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include "task.hpp"

class Household {
    public:
        Household(const std::string& config_file); // creates appliance configurations for the household, to be used in defining tasks
        const std::unordered_map<std::string, std::unique_ptr<Appliance>>& appliances() const;

    private:
        std::unordered_map<std::string, std::unique_ptr<Appliance>> appliances_;
};