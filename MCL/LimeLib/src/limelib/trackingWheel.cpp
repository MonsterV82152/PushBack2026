#include "limelib/trackingWheel.hpp"


limelib::TrackingWheel::TrackingWheel(pros::Rotation& sensor, real_t diameter, real_t offset)
    : sensor(sensor), diameter(diameter), offset(offset), lastPos(0) {
}

limelib::real_t limelib::TrackingWheel::getDistanceTravelled() {
    real_t currentPos = sensor.get_position();
    real_t distance = (currentPos - lastPos) * (diameter * M_PI / 360.0);
    lastPos = currentPos;
    return distance;
}