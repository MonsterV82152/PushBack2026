#pragma once

#ifndef DEVICES_HPP
#define DEVICES_HPP

#include "main.h"
#include "lemlib/api.hpp"
#include "misc/piston.hpp"
#include "misc/autonomous_selector.hpp"

#define MANIPULATOR_ID 1
#define DRIVETRAIN_ID 2

inline pros::MotorGroup leftDT({-8, 9, -10});
inline pros::MotorGroup rightDT({3, 4, -5});
inline pros::MotorGroup motors({2, -19});

inline pros::Rotation verticalTrackingWheel(1);
inline pros::Rotation horizontalTrackingWheel(20);

inline pros::Imu imu(11);

inline pros::ADIDigitalOut ptoPiston('D');
inline pros::ADIDigitalOut wingPiston('B');
inline pros::ADIDigitalOut liftPiston('C');
inline pros::ADIDigitalOut intakeLiftPiston('E');
inline pros::ADIDigitalOut scoringPistonADI('F');

inline Piston pto(&ptoPiston);
inline Piston wing(&wingPiston);
inline Piston lift(&liftPiston);
inline Piston intakeLift(&intakeLiftPiston);
inline Piston scoringPiston(&scoringPistonADI);
inline pros::ADIAnalogIn potentiometer('A');

inline lemlib::TrackingWheel verticalWheel(&verticalTrackingWheel, -2, 2);
inline lemlib::TrackingWheel horizontalWheel(&horizontalTrackingWheel, -2, -3);

inline lemlib::Drivetrain LEMLIB_drivetrain(&leftDT, &rightDT,
                                            10.85,
                                            lemlib::Omniwheel::NEW_275,
                                            600,
                                            1.95);

inline lemlib::OdomSensors LEMLIB_sensors(&verticalWheel, nullptr, &horizontalWheel, nullptr, &imu);
inline lemlib::ControllerSettings LEMLIB_lateral_controller(
    10,     // proportional gain (kP)
    0.1,    // integral gain (kI)
    67,     // derivative gain (kD)
    0.0392, // anti windup

    1,   // small error range, in inches
    100, // small error range timeout, in milliseconds
    3,   // large error range, in inches
    500, // large error range timeout, in milliseconds
    0    // maximum acceleration (slew)
);

inline lemlib::ControllerSettings LEMLIB_angular_controller(
    4.5, // proportional gain (kP)
    0,   // integral gain (kI)
    50,  // derivative gain (kD)
    // 5.3961, // anti windup
    0,

    1,   // small error range, in inches
    100, // small error range timeout, in milliseconds
    3,   // large error range, in inches
    500, // large error range timeout, in milliseconds
    0    // maximum acceleration (slew)
);

inline lemlib::Chassis chassis(LEMLIB_drivetrain,         // drivetrain settins
                               LEMLIB_lateral_controller, // lateral PID settings
                               LEMLIB_angular_controller, // angular PID settings
                               LEMLIB_sensors             // odometry sensors
);

#endif