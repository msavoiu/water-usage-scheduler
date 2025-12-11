class WaterSystem {
    public:
        WaterSystem();

        // Setters
        void setSupplyPerHour();
        void setGreywaterStorageCapacity();
        void setCurrentGreywaterStore();

        // Getters
        double supplyPerHour();
        double greywaterStorageCapacity();
        double currentGreywaterStore();

    private:
        double supply_per_hour;
        double greywater_storage_capacity;
        double current_greywater_store;
};
