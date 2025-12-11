#include <string>
#include <vector>

class Appliance {
    public:
        Appliance(
            const std::string& name,
            double cycle_time,
            const std::vector<double>& water_usage_per_minute,
            bool interruptable,
            bool takes_greywater
        );

        // Setters
        void setName(const std::string& name);
        void setCycleTime(double minutes);
        void setWaterUsagePerMinute(const std::vector<double>& liters);
        void setInterruptable(bool interruptable);
        void setTakesGreywater(bool takes_greywater);

        // Getters
        std::string name() const;
        double cycleTime() const;
        const std::vector<double>& waterUsagePerMinute() const;
        bool interruptable() const;
        bool takesGreywater() const;

    protected:
        std::string name_;
        double cycle_time_; // minutes
        std::vector<double> water_usage_per_minute_; // appliances usually have bursts of water usage
        bool interruptable_; // some appliances, like washing machine, should not be stopped and left to sit for extended periods of time
        bool takes_greywater_;
};

// Generates greywater for use by other appliances/systems
class GreywaterAppliance: public Appliance {
    public:
        GreywaterAppliance(
            const std::string& name,
            double cycle_time,
            const std::vector<double>& water_usage_per_minute,
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
