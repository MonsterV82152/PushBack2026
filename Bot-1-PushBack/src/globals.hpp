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


inline AutonSelector autonSelect;

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
inline pros::MotorGroup leftDT({-11, -12, 13}); // Change these ports to match your left drivetrain motors
inline pros::MotorGroup rightDT({14, 15, -16}); // Change these ports to match your left drivetrain motors
inline pros::Imu IMU(8);

inline pros::Motor bottom(-1);
inline pros::Motor middle(2);
inline pros::Motor top(-3);
inline pros::Motor bucket(-4);

inline pros::Optical topColor(5);

inline pros::ADIDigitalOut matchLoader('A');




namespace OdometryConfigs {
    struct PID {
        static constexpr double lateralKp = 1.0;
        static constexpr double lateralKi = 0.0;
        static constexpr double lateralKd = 25.0;
        static constexpr double angularKp = 1.0;
        static constexpr double angularKi = 0.0;
        static constexpr double angularKd = 50.0;
    };
    inline pros::Rotation vertical(-12); // Change this port to match your vertical tracking wheel rotation sensor
    inline pros::Rotation horizontal(-13); // Change this port to match your horizontal tracking wheel rotation sensor. Delete / Comment if not using

    inline lemlib::Drivetrain LEMLIB_drivetrain(&leftDT, &rightDT, 
        13, // Measure distance between left and right wheels in inches
        lemlib::Omniwheel::NEW_275, // Change to the wheel size you are using
        450, // Wheel RPM
        2 // Leave as 2 for now
    );
    inline lemlib::TrackingWheel LEMLIB_vertical_TW(&vertical, 
        2, // Tracking wheel diameter in inches
        0.22 // Distance from the tracking wheel to the center vertical axis in inches
    );
    inline lemlib::TrackingWheel LEMLIB_horizontal_TW(&horizontal, 
        2, // Tracking wheel diameter in inches
        1.5 // Distance from the tracking wheel to the center horizontal axis in inches
    ); // Delete / Comment if not using

    inline lemlib::OdomSensors LEMLIB_sensors(&LEMLIB_vertical_TW, nullptr, &LEMLIB_horizontal_TW, nullptr, &IMU);
    inline lemlib::ControllerSettings LEMLIB_lateral_controller(
        PID::lateralKp, // proportional gain (kP)
        PID::lateralKi, // integral gain (kI)
        PID::lateralKd, // derivative gain (kD)
        3,         // anti windup

        1,   // small error range, in inches
        100, // small error range timeout, in milliseconds
        3,   // large error range, in inches
        500, // large error range timeout, in milliseconds
        10   // maximum acceleration (slew)
    );

    inline lemlib::ControllerSettings LEMLIB_angular_controller(
        PID::angularKp, // proportional gain (kP)
        PID::angularKi, // integral gain (kI)
        PID::angularKd, // derivative gain (kD)
        0,         // anti windup
        0,         // small error range, in inches
        0,         // small error range timeout, in milliseconds
        0,         // large error range, in inches
        0,         // large error range timeout, in milliseconds
        0          // maximum acceleration (slew)
    );
}
inline lemlib::Chassis chassis(OdometryConfigs::LEMLIB_drivetrain,         // drivetrain settins
                               OdometryConfigs::LEMLIB_lateral_controller, // lateral PID settings
                               OdometryConfigs::LEMLIB_angular_controller, // angular PID settings
                               OdometryConfigs::LEMLIB_sensors             // odometry sensors
);

/*----------------------LEMLIB INIT----------------------*/



#endif