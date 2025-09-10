#pragma once

#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <string>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <deque>
#include <atomic>
#include "main.h"
#include "lemlib/api.hpp"
#include "sensor_loc.cpp"


/*----------------------Defines----------------------*/

// Constants
constexpr double PI = 3.141592;

/*---Controller---*/
inline pros::Controller master(pros::E_CONTROLLER_MASTER);
inline pros::Controller slave(pros::E_CONTROLLER_PARTNER);

/*------------------Global Variables------------------*/

namespace buttons {
    constexpr auto R1 = pros::E_CONTROLLER_DIGITAL_R1;
    constexpr auto R2 = pros::E_CONTROLLER_DIGITAL_R2;
    constexpr auto L1 = pros::E_CONTROLLER_DIGITAL_L1;
    constexpr auto L2 = pros::E_CONTROLLER_DIGITAL_L2;
    constexpr auto A = pros::E_CONTROLLER_DIGITAL_A;
    constexpr auto B = pros::E_CONTROLLER_DIGITAL_B;
    constexpr auto Y = pros::E_CONTROLLER_DIGITAL_Y;
    constexpr auto X = pros::E_CONTROLLER_DIGITAL_X;
    constexpr auto UP = pros::E_CONTROLLER_DIGITAL_UP;
    constexpr auto DOWN = pros::E_CONTROLLER_DIGITAL_DOWN;
    constexpr auto LEFT = pros::E_CONTROLLER_DIGITAL_LEFT;
    constexpr auto RIGHT = pros::E_CONTROLLER_DIGITAL_RIGHT;
}

const pros::motor_brake_mode_e_t brake = pros::E_MOTOR_BRAKE_BRAKE;
const pros::motor_brake_mode_e_t coast = pros::E_MOTOR_BRAKE_COAST;
const pros::motor_brake_mode_e_t hold = pros::E_MOTOR_BRAKE_HOLD;

/*-------------Define all configurations-------------*/
inline pros::MotorGroup leftDT({-1, -2, 3}); // Change these ports to match your left drivetrain motors
inline pros::MotorGroup rightDT({-8, 9, 10}); // Change these ports to match your right drivetrain motors
inline pros::Imu inertial(4);

namespace localization {
    inline pros::Distance rightDS(7);
    inline pros::Distance leftDS(6);
    inline pros::Distance frontDS(14);
    inline pros::Distance backDS(17);
}
inline dist_sensor rightLoc({&localization::rightDS, lemlib::Pose(3, -0.6, 90)});
inline dist_sensor leftLoc({&localization::leftDS, lemlib::Pose(-3.4, -0.6, 270)});
inline dist_sensor frontLoc({&localization::frontDS, lemlib::Pose(-4.13, 6, 0)});
inline dist_sensor backLoc({&localization::backDS, lemlib::Pose(4, -4.1, 180)});
inline pros::Rotation vertical(5);

std::vector<limelib::MCLDistance> mclSensors = {
    {localization::rightDS, limelib::Pose2D(3, -0.6, 90)},
    {localization::leftDS, limelib::Pose2D(-3.4, -0.6, 270)},
    {localization::frontDS, limelib::Pose2D(-4.13, 6, 0)},
    {localization::backDS, limelib::Pose2D(4, -4.1, 180)}
};
limelib::MCLDistance verticalMCLDist(localization::rightDS, limelib::Pose2D(3, -0.6, 90));
limelib::TrackingWheel verticalTW(vertical, 2.75, 0.0);
limelib::Field2D field(144, 144, {limelib::Rectangle2D(10,10,10,10)});
limelib::MCL mcl(&verticalTW, nullptr, inertial, mclSensors, field, 100, 10, 10);


#endif