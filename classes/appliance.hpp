#include <string>
#include <vector>

class Appliance {
    public:
        Appliance(
            const std::string& name,
            double cycle_time,
            const std::vector<double>& water_usage_per_minute,
            double total_water_usage,
            bool interruptable,
            bool takes_greywater
        );

        Appliance(const Appliance& other);

        // Setters
        void setName(const std::string& name);
        void setCycleTime(int minutes);
        void setWaterUsagePerMinute(const std::vector<double>& liters);
        void setTotalWaterUsage(double total_water_usage);
        void setInterruptable(bool interruptable);
        void setTakesGreywater(bool takes_greywater);
        void setProducesGreywater(bool produces_greywater);

        // Getters
        std::string name() const;
        int cycleTime() const;
        const std::vector<double>& waterUsagePerMinute() const;
        double totalWaterUsage() const;
        bool interruptable() const;
        bool takesGreywater() const;
        bool producesGreywater() const;

    protected:
        std::string name_;
        int cycle_time_; // minutes
        std::vector<double> water_usage_per_minute_; // appliances usually have bursts of water usage
        double total_water_usage_;
        bool interruptable_; // some appliances, like washing machine, should not be stopped and left to sit for extended periods of time
        bool takes_greywater_;
        bool produces_greywater_;
};

// generates greywater for use by other appliances/systems
class GreywaterAppliance: public Appliance {
    public:
        GreywaterAppliance(
            const std::string& name,
            int cycle_time,
            const std::vector<double>& water_usage_per_minute,
            double total_water_usage,
            bool interruptable,
            double water_output_per_cycle
        );

        // Setters
        void setWaterOutputPerCycle(double water_output_per_cycle);

        // Getters
        double waterOutputPerCycle() const;  

    private:
        double water_output_per_cycle_; // liters
};
