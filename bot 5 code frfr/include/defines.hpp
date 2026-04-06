#ifndef DEFINES_HPP
#define DEFINES_HPP

#include "main.h"
#include "lemlib/api.hpp"
#include "autonomous_selector.hpp"
#include "sensor_loc.hpp"
#include "piston.hpp"

inline AutonSelector autonSelect;

inline pros::MotorGroup leftDT({-8, 9, -10});  // Change these ports to match your left drivetrain motors
inline pros::MotorGroup rightDT({3, 4, -5}); // Change these ports to match your left drivetrain motors
inline pros::MotorGroup motors({2, 19});
inline pros::Imu imu(21);

inline pros::Rotation vr(1);
inline pros::Rotation hr(20);

inline pros::ADIAnalogIn pot(1);
inline pros::ADIDigitalOut liftPiston(2);
inline pros::ADIDigitalOut intakeLiftPiston(3);
inline pros::ADIDigitalOut wingPiston(4);
inline pros::ADIDigitalOut matchLoadPiston(5);
inline pros::ADIDigitalOut hoodPiston(6);
inline pros::ADIDigitalOut ptoPiston(7);
inline pros::ADIDigitalOut leverPiston(8);

inline pros::Distance lds(11);
inline pros::Distance rds(12);
inline pros::Distance bds(18);

inline Piston matchLoader(&matchLoadPiston);
inline Piston pto(&ptoPiston);
inline Piston intakeLift(&intakeLiftPiston);
inline Piston lift(&liftPiston);
inline Piston wing(&wingPiston);
inline Piston hood(&hoodPiston);
inline Piston lever(&leverPiston);

inline lemlib::TrackingWheel vtc(&vr, 1.975, -2);
inline lemlib::TrackingWheel htc(&hr, 1.975, 2.25);


inline const int SCORING_DOWN_ANGLE = 1800;
inline const int SCORING_UP_ANGLE = 3000;
inline const int SCORING_TIMEOUT = 500;

inline bool scoring = false;

inline int prevIntakeSpeed = 0;
inline int intakeState = 0;







inline lemlib::Drivetrain LEMLIB_drivetrain(&leftDT, &rightDT,
                                            10.125,
                                            lemlib::Omniwheel::NEW_325,
                                            450,
                                            1.9);

inline lemlib::OdomSensors LEMLIB_sensors(&vtc, nullptr, &htc, nullptr, &imu);

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