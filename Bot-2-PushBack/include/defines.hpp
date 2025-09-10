#pragma once

#ifndef DEFINES_HPP
#define DEFINES_HPP

#include "main.h"
#include "lemlib/api.hpp"
#include "autonomous_selector.hpp"
#include "sensor_loc.hpp"
#include "movements.hpp"
#include "piston.hpp"

inline AutonSelector autonSelect;

inline pros::MotorGroup leftDT({-1, -2, -3}); 
inline pros::MotorGroup rightDT({8, 9, 10}); 
inline pros::Imu imu(13);

inline pros::Motor front(16);
inline pros::Motor back(-19);
inline pros::Motor middle(6);
inline pros::Motor intake(-18);

inline pros::Distance frontDS(7);
inline pros::Distance intakeDS(15);
inline pros::Optical middleCS(14);

inline pros::Rotation trackingVertical(12);

inline pros::ADIDigitalOut flipPiston('D');
inline pros::ADIDigitalOut blockerPiston('C');
inline pros::ADIDigitalOut matchLoaderPiston('B');
inline pros::ADIDigitalOut parkPiston('A');

inline Piston blocker(&blockerPiston);
inline Piston flip(&flipPiston);
inline Piston park(&parkPiston);
inline Piston matchLoader(&matchLoaderPiston);

inline Roller roller(front, middle, intake, back, flip, blocker);

inline Robot robot(roller, matchLoader, master);

inline lemlib::Drivetrain LEMLIB_drivetrain(&leftDT, &rightDT,
                                            12,
                                            lemlib::Omniwheel::NEW_325,
                                            450,
                                            2);

inline lemlib::TrackingWheel LEMLIB_tracking_vertical(&trackingVertical, lemlib::Omniwheel::NEW_275, 0);

inline lemlib::OdomSensors LEMLIB_sensors(&LEMLIB_tracking_vertical, nullptr, nullptr, nullptr, &imu);
inline lemlib::ControllerSettings LEMLIB_lateral_controller(
    6,    // proportional gain (kP)
    0,     // integral gain (kI)
    65,    // derivative gain (kD)
    0.035, // anti windup

    1,   // small error range, in inches
    100, // small error range timeout, in milliseconds
    3,   // large error range, in inches
    500, // large error range timeout, in milliseconds
    10   // maximum acceleration (slew)
);

inline lemlib::ControllerSettings LEMLIB_angular_controller(
    2,  // proportional gain (kP)
    0,  // integral gain (kI)
    17, // derivative gain (kD)
    2,  // anti windup

    0, // small error range, in inches
    0, // small error range timeout, in milliseconds
    0, // large error range, in inches
    0, // large error range timeout, in milliseconds
    0  // maximum acceleration (slew)
);

inline lemlib::Chassis chassis(LEMLIB_drivetrain,         // drivetrain settins
                               LEMLIB_lateral_controller, // lateral PID settings
                               LEMLIB_angular_controller, // angular PID settings
                               LEMLIB_sensors             // odometry sensors
);

#endif