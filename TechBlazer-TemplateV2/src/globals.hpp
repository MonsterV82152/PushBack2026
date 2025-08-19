#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include "main.h"
#include <string>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include "AutonSelector.hpp"
#include "lemlib/api.hpp"
#include "sensor_loc.cpp"
#include "piston.cpp"

inline AutonSelector autonSelect;

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
inline pros::MotorGroup leftDT({-1, -2, 3});  // Change these ports to match your left drivetrain motors
inline pros::MotorGroup rightDT({-8, 9, 10}); // Change these ports to match your left drivetrain motors
inline pros::Imu imu(4);

namespace OdometryConfigs
{

    inline lemlib::Drivetrain LEMLIB_drivetrain(&leftDT, &rightDT,
                                                12,
                                                lemlib::Omniwheel::NEW_325,
                                                450,
                                                2);
    inline lemlib::TrackingWheel LEMLIB_vertical_TW(&vertical,
                                                    lemlib::Omniwheel::NEW_275,
                                                    0                           // Distance from the tracking wheel to the center vertical axis in inches
    );

    inline lemlib::OdomSensors LEMLIB_sensors(&LEMLIB_vertical_TW, nullptr, nullptr, nullptr, &imu);
    inline lemlib::ControllerSettings LEMLIB_lateral_controller(
        10, // proportional gain (kP)
        0, // integral gain (kI)
        20, // derivative gain (kD)
        0.035,          // anti windup

        1,   // small error range, in inches
        100, // small error range timeout, in milliseconds
        3,   // large error range, in inches
        500, // large error range timeout, in milliseconds
        10   // maximum acceleration (slew)
    );

    inline lemlib::ControllerSettings LEMLIB_angular_controller(
        2, // proportional gain (kP)
        0, // integral gain (kI)
        10, // derivative gain (kD)
        2,          // anti windup

        0, // small error range, in inches
        0, // small error range timeout, in milliseconds
        0, // large error range, in inches
        0, // large error range timeout, in milliseconds
        0  // maximum acceleration (slew)
    );
}
inline lemlib::Chassis chassis(OdometryConfigs::LEMLIB_drivetrain,         // drivetrain settins
                               OdometryConfigs::LEMLIB_lateral_controller, // lateral PID settings
                               OdometryConfigs::LEMLIB_angular_controller, // angular PID settings
                               OdometryConfigs::LEMLIB_sensors             // odometry sensors
);

/*----------------------LEMLIB INIT----------------------*/

#endif