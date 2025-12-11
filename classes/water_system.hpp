class WaterSystem {
    public:
        WaterSystem(
            double water_supply_per_hour,
            double greywater_storage_capacity,
            double current_greywater_store
        );

        // Setters
        void setWaterSupplyPerHour(double water_supply_per_hour);
        void setGreywaterStorageCapacity(double greywater_storage_capacity);
        void setCurrentGreywaterStore(double current_greywater_store);

        // Getters
        double waterSupplyPerHour() const;
        double greywaterStorageCapacity() const;
        double currentGreywaterStore() const;

    private:
        double water_supply_per_hour_;
        double greywater_storage_capacity_;
        double current_greywater_store_;
};
