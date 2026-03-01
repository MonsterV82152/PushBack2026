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
inline pros::ADIAnalogIn potentiometer('A');
inline pros::Rotation vTrack(4);
inline pros::Rotation hTrack(5);

// loc sensors
inline pros::Distance LOCF(7);
inline pros::Distance LOCB(10);
inline pros::Distance LOCL(11);
inline pros::Distance LOCR(9);

// loc sensor wrappers with robot relative poses (x,y,theta);
inline dist_sensor LR(LOCR, lemlib::Pose(5.5, -3.25, 90));
inline dist_sensor LL(LOCL, lemlib::Pose(-4, 3.5, -90));
inline dist_sensor LF(LOCF, lemlib::Pose(-4.25, -3.5, 180));
inline dist_sensor LB(LOCB, lemlib::Pose(4, 2.5, 0));

// pistons
inline pros::ADIDigitalOut matchLoaderPiston('D');
inline pros::ADIDigitalOut ptoPiston('C'); // hood as well
inline pros::ADIDigitalOut ptoPiston2('B');
inline pros::ADIDigitalOut intakeLiftPiston('E');
inline pros::ADIDigitalOut liftPiston('G');
inline pros::ADIDigitalOut wingPiston('H');

// piston wrappers
inline Piston matchLoader(&matchLoaderPiston);
inline Piston pto(&ptoPiston, &ptoPiston2);
inline Piston intakeLift(&intakeLiftPiston);
inline Piston lift(&liftPiston);
inline Piston wing(&wingPiston);

// inline lemlib::TrackingWheel verticalWheel(&vTrack, 1.975, 0.05);
// inline lemlib::TrackingWheel horizontalWheel(&hTrack, 1.975, -4.75);

inline lemlib::Drivetrain LEMLIB_drivetrain(&leftDT, &rightDT,
                                            10.85,
                                            lemlib::Omniwheel::NEW_275,
                                            600,
                                            1.95);

inline lemlib::OdomSensors LEMLIB_sensors(nullptr, nullptr, nullptr, nullptr, &imu);
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