#pragma once

#ifndef UTILS_CPP
#define UTILS_CPP

#include "globals.hpp"

void debugOdom()
{
    pros::lcd::initialize();
    pros::Task screen_task([&]()
                           {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // theta
            // delay to save resources
            pros::delay(20);
        } });
}


#endif