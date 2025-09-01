#ifndef DEFINES_HPP
#define DEFINES_HPP

#include "main.h"
#include "lemlib/api.hpp"
#include "autonomous_selector.hpp"
#include "sensor_loc.hpp"
#include "piston.hpp"

inline AutonSelector autonSelect;

inline pros::MotorGroup leftDT({-1, -2, 3});  // Change these ports to match your left drivetrain motors
inline pros::MotorGroup rightDT({-8, 9, 10}); // Change these ports to match your left drivetrain motors
inline pros::Imu imu(4);


inline pros::Motor front(4);
inline pros::Motor back(5);
inline pros::Motor middle(6);


inline lemlib::Drivetrain LEMLIB_drivetrain(&leftDT, &rightDT,
                                            12,
                                            lemlib::Omniwheel::NEW_325,
                                            450,
                                            2);

inline lemlib::OdomSensors LEMLIB_sensors(nullptr, nullptr, nullptr, nullptr, &imu);
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