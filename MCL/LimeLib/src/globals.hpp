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
inline const bool odometry = true; // Set to true to use odometry, false to use MCL
std::vector<MCLDistance> mclSensors = {
    {localization::rightDS, Pose2D(-4.25, -2.25, 90)},
    {localization::leftDS, Pose2D(4.25, -2.25, 270)},
    {localization::frontDS, Pose2D(4.5, 3, 0)},
    {localization::backDS, Pose2D(-3.5, -5.5, 180)}};
TrackingWheel verticalTW(vertical, 2.75, -0.25);
Field2D field(144.0f, 144.0f, {std::make_shared<Circle2D>(67.5f, 48.0f, 4.17f), std::make_shared<Circle2D>(-67.5f, 48.0f, 4.17f), std::make_shared<Circle2D>(67.5f, -48.0f, 4.17f), std::make_shared<Circle2D>(-67.5f, -48.0f, 4.17f)});
;
Locator *locator = odometry ? static_cast<Locator *>(new Odometry(&verticalTW, nullptr, inertial)) : static_cast<Locator *>(new MCL(&verticalTW, nullptr, inertial, mclSensors, field, 100, 0.1, 0.1, true));

#endif