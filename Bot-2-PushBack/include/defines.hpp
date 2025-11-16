#pragma once

#ifndef DEFINES_HPP
#define DEFINES_HPP

#include "main.h"
#include "lemlib/api.hpp"
#include "autonomous_selector.hpp"
#include "sensor_loc.hpp"
#include "movements.hpp"
#include "piston.hpp"

inline AutonSelector autonSelect; // Autonomous selector object

// 
inline pros::MotorGroup leftDT({-1, -2, -3});
inline pros::MotorGroup rightDT({8, 9, 10});
inline pros::Imu imu(13);

inline pros::Motor front(16);
inline pros::Motor back(-19);
inline pros::Motor middle(6);
inline pros::Motor intake(-18);

inline pros::Distance frontDS(7);
inline pros::Distance intakeDS(15);
inline pros::Optical middleCS(12);

inline pros::Distance LOCF(21);
inline pros::Distance LOCB(4);
inline pros::Distance LOCR(20);
inline pros::Distance LOCL(5);

inline dist_sensor LF(&LOCF, lemlib::Pose(4.7, 4.5, 0));
inline dist_sensor LB(&LOCB, lemlib::Pose(-3.5, -5.5, 180));
inline dist_sensor LR(&LOCR, lemlib::Pose(4.25, -2.25, 90));
inline dist_sensor LL(&LOCL, lemlib::Pose(-4.25, -2.25, 270));

inline pros::Rotation trackingVertical(14);

inline pros::ADIDigitalOut flipPiston('C');
inline pros::ADIDigitalOut blockerPiston('D');
inline pros::ADIDigitalOut matchLoaderPiston('B');
inline pros::ADIDigitalOut parkPiston('A');
inline pros::ADIDigitalOut intakeLiftPiston('H');

inline Piston blocker(&blockerPiston);
inline Piston flip(&flipPiston);
inline Piston park(&parkPiston);
inline Piston matchLoader(&matchLoaderPiston);
inline Piston intakeLift(&intakeLiftPiston);

inline Roller roller(front, middle, intake, back, flip, blocker, intakeLift);

inline Robot robot(roller, matchLoader, master);

inline lemlib::Drivetrain LEMLIB_drivetrain(&leftDT, &rightDT,
                                            12,
                                            lemlib::Omniwheel::NEW_325,
                                            450,
                                            2);

inline lemlib::TrackingWheel LEMLIB_tracking_vertical(&trackingVertical, lemlib::Omniwheel::NEW_275, -0.25);

inline lemlib::OdomSensors LEMLIB_sensors(&LEMLIB_tracking_vertical, nullptr, nullptr, nullptr, &imu);
inline lemlib::ControllerSettings LEMLIB_lateral_controller(
    6,     // proportional gain (kP)
    0.5,   // integral gain (kI)
    71,    // derivative gain (kD)
    1.285, // anti windup

    1,   // small error range, in inches
    100, // small error range timeout, in milliseconds
    3,   // large error range, in inches
    500, // large error range timeout, in milliseconds
    10   // maximum acceleration (slew)
);

inline lemlib::ControllerSettings LEMLIB_angular_controller(
    3,       // proportional gain (kP)
    0.94,    // integral gain (kI)
    25.1,    // derivative gain (kD)
    1.38734, // anti windup

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