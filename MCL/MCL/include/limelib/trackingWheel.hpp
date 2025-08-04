#ifndef TRACKINGWHEEL_HPP
#define TRACKINGWHEEL_HPP

#include "limelib/types.hpp"
#include "pros/rotation.hpp"

namespace limelib
{
    class TrackingWheel
    {
    public:
        // Allow nullptr construction with default values
        TrackingWheel(pros::Rotation *sensor = nullptr, real_t diameter = 0, real_t offset = 0);
        
        // Check if wheel is enabled (has valid sensor)
        bool isEnabled() const { return sensor != nullptr; }
        
        // Return 0 if sensor is nullptr
        real_t getDistanceTravelled() {
            return isEnabled() ? calculateDistance() : 0;
        }
        
        real_t getOffset() const { return offset; }
        real_t getDiameter() const { return diameter; }

    private:
        pros::Rotation *sensor;
        real_t diameter;
        real_t offset;
        real_t lastPos;
        
        // Helper method to calculate actual distance
        real_t calculateDistance();
    };
}

#endif