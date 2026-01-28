#pragma once

#include <string>
#include <vector>

class Appliance {
    public:
        Appliance(
            const std::string& name,
            double cycle_time,
            double water_usage_per_minute,
            bool interruptable,
            bool takes_greywater
        );

        Appliance(const Appliance& other);

        virtual ~Appliance() = default;

        // Setters
        void setName(const std::string& name);
        void setCycleTime(int minutes);
        void setWaterUsagePerMinute(double water_usage_per_minute);
        void setInterruptable(bool interruptable);
        void setTakesGreywater(bool takes_greywater);
        void setProducesGreywater(bool produces_greywater);
        void setGreywaterOutputPerCycle(bool greywater_output_per_cycle);

        // Getters
        std::string name() const;
        int cycleTime() const;
        double waterUsagePerMinute() const;
        bool interruptable() const;
        bool takesGreywater() const;
        bool producesGreywater() const;

        // default: produces no greywater
        virtual double waterOutputPerCycle() const {
            return 0.0;
        }
        

    protected:
        std::string name_;
        int cycle_time_; // minutes
        double water_usage_per_minute_;
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
            double water_usage_per_minute,
            bool interruptable,
            bool takes_greywater,
            double water_output_per_cycle
        );

        // Setters
        void setWaterOutputPerCycle(double water_output_per_cycle);

        // Getters
        double waterOutputPerCycle() const override;  

    private:
        double water_output_per_cycle_; // liters
};
