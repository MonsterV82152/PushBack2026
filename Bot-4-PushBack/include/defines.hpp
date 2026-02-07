#ifndef DEFINES_HPP
#define DEFINES_HPP

#include "main.h"
#include "lemlib/api.hpp"
#include "autonomous_selector.hpp"
#include "sensor_loc.hpp"
#include "piston.hpp"

inline AutonSelector autonSelect;

// Drivetrain motors
inline pros::MotorGroup leftDT({-12, 13, -14});
inline pros::MotorGroup rightDT({18, 17, -20});

// System motors
inline pros::MotorGroup systemMotors({-15, 19}, pros::MotorGearset::red, pros::MotorUnits::degrees);

// sensors
inline pros::Imu imu(16);
inline pros::Imu leverImu(9);
inline pros::Rotation vTrack(4);
inline pros::Rotation hTrack(-5);

// loc sensors
inline pros::Distance LOCF(7);
inline pros::Distance LOCB(10);
inline pros::Distance LOCL(11);
inline pros::Distance LOCR(8);

// loc sensor wrappers with robot relative poses (x,y,theta);
inline dist_sensor LF(&LOCF, lemlib::Pose(3.5, 5.0, 0));
inline dist_sensor LB(&LOCB, lemlib::Pose(-3.5, 5.0, 0));
inline dist_sensor LR(&LOCL, lemlib::Pose(0, -5.0, -90));
inline dist_sensor LL(&LOCR, lemlib::Pose(0, 5.0, 90));

// pistons
inline pros::ADIDigitalOut hoodPiston('A');
inline pros::ADIDigitalOut matchLoadPiston('C');
inline pros::ADIDigitalOut ptoPiston('B');
inline pros::ADIDigitalOut intakeLiftPiston('D');
inline pros::ADIDigitalOut liftPiston('F');
inline pros::ADIDigitalOut wingPiston('G');

// piston wrappers
inline Piston hood(&hoodPiston);
inline Piston matchLoad(&matchLoadPiston);
inline Piston pto(&ptoPiston);
inline Piston intakeLift(&intakeLiftPiston);
inline Piston lift(&liftPiston);
inline Piston wing(&wingPiston);

inline lemlib::TrackingWheel verticalWheel(&vTrack, lemlib::Omniwheel::NEW_275, 0);
inline lemlib::TrackingWheel horizontalWheel(&hTrack, lemlib::Omniwheel::NEW_275, -5);

inline lemlib::Drivetrain LEMLIB_drivetrain(&leftDT, &rightDT,
                                            10.85,
                                            lemlib::Omniwheel::NEW_275,
                                            600,
                                            2);

inline lemlib::OdomSensors LEMLIB_sensors(&verticalWheel, nullptr, &horizontalWheel, nullptr, &imu);
inline lemlib::ControllerSettings LEMLIB_lateral_controller(
    10,    // proportional gain (kP)
    0,     // integral gain (kI)
    20,    // derivative gain (kD)
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
    10, // derivative gain (kD)
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