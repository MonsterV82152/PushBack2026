#ifndef DEFINES_HPP
#define DEFINES_HPP

#include "main.h"
#include "lemlib/api.hpp"
#include "autonomous_selector.hpp"
#include "sensor_loc.hpp"
#include "piston.hpp"

inline AutonSelector autonSelect;

// Drivetrain motors
inline pros::MotorGroup leftDT({1, 2, 3});
inline pros::MotorGroup rightDT({4, 5, 6});

// System motors
inline pros::MotorGroup systemMotors({7, 8});

// sensors
inline pros::Imu imu(9);
inline pros::Rotation vTrack(10);
inline pros::Rotation hTrack(11);

// loc sensors
inline pros::Distance LOCF(12);
inline pros::Distance LOCB(13);
inline pros::Distance LOCL(14);
inline pros::Distance LOCR(15);

// loc sensor wrappers with robot relative poses (x,y,theta);
inline dist_sensor LF(&LOCF, lemlib::Pose(3.5, 5.0, 0));
inline dist_sensor LB(&LOCB, lemlib::Pose(-3.5, 5.0, 0));
inline dist_sensor LR(&LOCL, lemlib::Pose(0, -5.0, -90));
inline dist_sensor LL(&LOCR, lemlib::Pose(0, 5.0, 90));

// pistons
inline pros::ADIDigitalOut lift('A');
inline pros::ADIDigitalOut matchLoad('B');
inline pros::ADIDigitalOut pto('C');
inline pros::ADIDigitalOut intakeLift('D');
inline pros::ADIDigitalOut hood('E');

// piston wrappers
inline Piston liftPiston(&lift);
inline Piston matchLoadPiston(&matchLoad);
inline Piston ptoPiston(&pto);
inline Piston intakeLiftPiston(&intakeLift);
inline Piston hoodPiston(&hood);


inline lemlib::TrackingWheel verticalWheel(&vTrack, lemlib::Omniwheel::NEW_275, 0);
inline lemlib::TrackingWheel horizontalWheel(&hTrack, lemlib::Omniwheel::NEW_275, 0);



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