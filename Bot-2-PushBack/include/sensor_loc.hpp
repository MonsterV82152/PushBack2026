#ifndef SENSORLOC_CPP
#define SENSORLOC_CPP

#include "main.h"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"
#include <math.h>
/**
 * @brief A struct to hold the distance sensor and its offset
 */
struct dist_sensor {
    pros::Distance *sensor;
    lemlib::Pose offset;
};
/**
 * @brief A function to correct the position of the robot using a distance sensor
 * @param sensor The distance sensor to use
 * @param chassis The chassis to correct
 * @param x If true, correct the x position, otherwise correct the y position
 * @param correct_rate The rate at which to correct the position
 * @param forced If true, force the correction even if the distance is not valid
 */
void correct_position(dist_sensor sensor, lemlib::Chassis *chassis, bool x, bool forced = true, double correct_rate = 5);

#endif
