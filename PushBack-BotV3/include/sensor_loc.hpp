#ifndef SENSORLOC_CPP
#define SENSORLOC_CPP

#include "main.h"
#include <math.h>

/**
 * @brief A function to correct the position of the robot using a distance sensor
 * @param sensor The distance sensor to use
 * @param locator The locator to correct
 * @param x If true, correct the x position, otherwise correct the y position
 * @param correct_rate The rate at which to correct the position
 * @param forced If true, force the correction even if the distance is not valid
 */
void correct_position(limelib::MCLDistance sensor, limelib::Locator *locator, bool x, bool forced = false, double correct_rate = 10);

#endif
