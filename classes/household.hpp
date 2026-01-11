#pragma once

#include <vector>
#include <memory>

#include "task.hpp"

class Household {
    public:
        Household(const std::string& config_file);
        const std::vector<std::unique_ptr<Appliance>>& getAppliances() const;

    private:
        std::vector<std::unique_ptr<Appliance>> appliances_;
};