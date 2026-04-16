#ifndef DEFINES_HPP
#define DEFINES_HPP

#include "main.h"
#include "lemlib/api.hpp"
#include "autonomous_selector.hpp"
#include "sensor_loc.hpp"
#include "piston.hpp"

inline AutonSelector autonSelect;

// Drivetrain motors
inline pros::MotorGroup leftDT({-8, 9, -10});
inline pros::MotorGroup rightDT({3, 4, -5});

// System motors
inline pros::MotorGroup motors({2, 19});

// sensors
inline pros::Imu imu(21);
inline pros::adi::AnalogIn pot('A');
inline pros::Rotation vrt(1);
inline pros::Rotation hrt(-20);

// loc sensors
inline pros::Distance LOCB(18);
inline pros::Distance LOCL(11);
inline pros::Distance LOCR(12);
inline pros::Distance LOCF(7);

// loc sensor wrappers with robot relative poses (x,y,theta);
inline dist_sensor LR(LOCR, lemlib::Pose(3.25, 4.75, 90), 0);
inline dist_sensor LL(LOCL, lemlib::Pose(-3.25, 4.75, -90), 0);
inline dist_sensor LB(LOCB, lemlib::Pose(1.625, -5, 180), 0);
inline dist_sensor LF(LOCF, lemlib::Pose(-3.25, 8.5, 0), 0.19198622);

// pistons
inline pros::adi::DigitalOut matchLoaderPiston('E');
inline pros::adi::DigitalOut ptoPiston('G');
inline pros::adi::DigitalOut intakeLiftPiston('C');
inline pros::adi::DigitalOut liftPiston('B');
inline pros::adi::DigitalOut wingPiston('D');
inline pros::adi::DigitalOut leverPiston('H');
inline pros::adi::DigitalOut hoodPiston('F');
// piston wrappers
inline Piston matchLoader(&matchLoaderPiston);
inline Piston pto(&ptoPiston);
inline Piston intakeLift(&intakeLiftPiston);
inline Piston lift(&liftPiston);
inline Piston wing(&wingPiston);
inline Piston lever(&leverPiston);
inline Piston hood(&hoodPiston);

// inline lemlib::TrackingWheel verticalWheel(&vTrack, 1.975, 0.05);
// inline lemlib::TrackingWheel horizontalWheel(&hTrack, 1.975, -4.75);

inline lemlib::Drivetrain LEMLIB_drivetrain(&leftDT, &rightDT,
                                            10.125,
                                            3.3,
                                            450,
                                            1.95);

// inline lemlib::TrackingWheel vtc(&vrt, 1.975, -1.625);
inline lemlib::TrackingWheel htc(&hrt, 1.979, -1.375);

inline lemlib::OdomSensors LEMLIB_sensors(nullptr, nullptr, &htc, nullptr, &imu);

inline lemlib::ControllerSettings LEMLIB_lateral_controller(
    5.6, // proportional gain (kP)
    0,   // integral gain (kI)
    29,  // derivative gain (kD)
    3,   // anti windup
    1,   // small error range, in inches
    100, // small error range timeout, in milliseconds
    3,   // large error range, in inches
    500, // large error range timeout, in milliseconds
    5);
inline lemlib::ControllerSettings LEMLIB_angular_controller(
    2.5, // proportional gain (kP)
    0,   // integral gain (kI)
    17,  // derivative gain (kD)
    3,   // anti windup
    1,   // small error range, in inches
    100, // small error range timeout, in milliseconds
    3,   // large error range, in inches
    500, // large error range timeout, in milliseconds
    5);

inline lemlib::Chassis chassis(LEMLIB_drivetrain,         // drivetrain settins
                               LEMLIB_lateral_controller, // lateral PID settings
                               LEMLIB_angular_controller, // angular PID settings
                               LEMLIB_sensors             // odometry sensors
);

#endif