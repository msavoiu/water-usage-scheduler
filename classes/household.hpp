#include <vector>
#include <memory>

#include "task.hpp"

class Household {
    public:
        Household(const std::string& config_file);

    private:
        std::vector<std::unique_ptr<Task>> tasks;
};