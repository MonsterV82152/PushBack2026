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

using namespace limelib;
/*----------------------Defines----------------------*/

// Constants
constexpr double PI = 3.141592;

/*---Controller---*/
inline pros::Controller master(pros::E_CONTROLLER_MASTER);
inline pros::Controller slave(pros::E_CONTROLLER_PARTNER);

/*------------------Global Variables------------------*/

namespace buttons
{
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
inline pros::MotorGroup leftDT({-1, -2, -3}); // Change these ports to match your left drivetrain motors
inline pros::MotorGroup rightDT({8, 9, 10});  // Change these ports to match your right drivetrain motors
inline pros::Imu inertial(13);

namespace localization
{
    inline pros::Distance rightDS(20);
    inline pros::Distance leftDS(5);
    inline pros::Distance frontDS(17);
    inline pros::Distance backDS(4);
}
inline pros::Rotation vertical(14);

std::vector<limelib::MCLDistance> mclSensors = {
    {localization::rightDS, limelib::Pose2D(-4.25, -2.25, 90)},
    {localization::leftDS, limelib::Pose2D(4.25, -2.25, 270)},
    {localization::frontDS, limelib::Pose2D(4.5, 3, 0)},
    {localization::backDS, limelib::Pose2D(-3.5, -5.5, 180)}};
TrackingWheel verticalTW(vertical, 2.75, -0.25);
Field2D field(144, 144, {Circle2D(67.5, 48, 4.17), Circle2D(-67.5, 48, 4.17), Circle2D(67.5, -48, 4.17), Circle2D(-67.5, -48, 4.17)});
MCL mcl(&verticalTW, nullptr, inertial, mclSensors, field, 100, 0.1, 0.1, true);

#endif