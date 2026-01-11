#pragma once

#include <atomic>

#include "appliance.hpp"

enum TaskStatus {
    NEW,
    RUNNING,
    WAITING,
    READY,
    TERMINATED
};

class Task {
    public:
        Task(
            Appliance& appliance,
            TaskStatus status_,
            int base_priority
        );

        bool finished(); 
        void runFor(int seconds);

        // Getters
        Appliance& appliance() const;
        TaskStatus status() const;
        int basePriority() const;
        int priority() const;
        double timeRemaining() const;
        double arrivalTime() const;
        bool canPreempt() const;
        int preemptions() const;
        int id() const;

        // Setters
        void setAppliance(Appliance& appliance);
        void setStatus(TaskStatus status);
        void setBasePriority(int base_priority);
        void setPriority(int priority);
        void setTimeRemaining(double time_remaining);
        void setArrivalTime(double arrival_time);
        void setCanPreempt(bool can_preempt);
        void setPreemptions(bool preemptions);
        void setId(int id);

        bool operator>(const Task& other) const; 

    private:
        Appliance& appliance_;

        TaskStatus status_;
        int base_priority_;
        int priority_;
        double arrival_time_; // minutes since midnight
        double time_remaining_;
        bool can_preempt_;
        int preemptions_;

        int id_;
        static std::atomic<int> next_id_;
};

// HELPERS
struct TaskCompare {
    bool operator()(const Task* a, const Task* b) const;
};

int statusRank(TaskStatus s);
bool isActive(TaskStatus s);
std::string statusToString(TaskStatus s);
std::string formatTime(double minutes);

bool taskPrintCompare(const Task* a, const Task* b);
