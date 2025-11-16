/**
 * @file defines.hpp
 * @brief Global robot hardware definitions and configurations
 *
 * This file contains all hardware component definitions, including:
 * - Motor and motor group configurations for drivetrain and mechanisms
 * - Sensor definitions (IMU, distance, optical, rotation sensors)
 * - Pneumatic piston assignments
 * - LemLib chassis and odometry configuration
 * - PID controller tuning parameters
 *
 * All objects are defined as inline variables for global access throughout
 * the codebase. Port numbers and configurations should be updated here when
 * hardware is modified.
 */

#pragma once

#ifndef DEFINES_HPP
#define DEFINES_HPP

#include "main.h"
#include "lemlib/api.hpp"
#include "autonomous_selector.hpp"
#include "sensor_loc.hpp"
#include "movements.hpp"
#include "piston.hpp"

// ============================================================================
// Autonomous Selector
// ============================================================================

inline AutonSelector autonSelect; ///< Autonomous selector for pre-match routine selection

// ============================================================================
// Drivetrain Motors
// ============================================================================

inline pros::MotorGroup leftDT({-1, -2, -3}); ///< Left drivetrain motor group (ports 1, 2, 3 - reversed)
inline pros::MotorGroup rightDT({8, 9, 10});  ///< Right drivetrain motor group (ports 8, 9, 10)
inline pros::Imu imu(13);                     ///< Inertial sensor for heading/orientation (port 13)

// ============================================================================
// Roller System Motors
// ============================================================================

inline pros::Motor front(16);   ///< Front roller motor (port 16)
inline pros::Motor back(-19);   ///< Back roller motor (port 19 - reversed)
inline pros::Motor middle(6);   ///< Middle roller motor (port 6)
inline pros::Motor intake(-18); ///< Intake roller motor (port 18 - reversed)

// ============================================================================
// Sensors - Distance and Optical
// ============================================================================

inline pros::Distance frontDS(7);   ///< Front distance sensor for obstacle detection (port 7)
inline pros::Distance intakeDS(15); ///< Intake distance sensor for ring detection (port 15)
inline pros::Optical middleCS(12);  ///< Middle color/optical sensor for ring sorting (port 12)

// ============================================================================
// Localization Distance Sensors
// ============================================================================

inline pros::Distance LOCF(21); ///< Front localization distance sensor (port 21)
inline pros::Distance LOCB(4);  ///< Back localization distance sensor (port 4)
inline pros::Distance LOCR(20); ///< Right localization distance sensor (port 20)
inline pros::Distance LOCL(5);  ///< Left localization distance sensor (port 5)

// Distance sensor wrappers with robot-relative poses
inline dist_sensor LF(&LOCF, lemlib::Pose(4.7, 4.5, 0));       ///< Front sensor at (4.7", 4.5") facing forward
inline dist_sensor LB(&LOCB, lemlib::Pose(-3.5, -5.5, 180));   ///< Back sensor at (-3.5", -5.5") facing backward
inline dist_sensor LR(&LOCR, lemlib::Pose(4.25, -2.25, 90));   ///< Right sensor at (4.25", -2.25") facing right
inline dist_sensor LL(&LOCL, lemlib::Pose(-4.25, -2.25, 270)); ///< Left sensor at (-4.25", -2.25") facing left

// ============================================================================
// Tracking Wheels
// ============================================================================

inline pros::Rotation trackingVertical(14); ///< Vertical tracking wheel rotation sensor (port 14)  ///< Vertical tracking wheel rotation sensor (port 14)

// ============================================================================
// Pneumatic Pistons (ADI Ports)
// ============================================================================

inline pros::ADIDigitalOut flipPiston('C');        ///< Flip piston for ring redirection (ADI port C)
inline pros::ADIDigitalOut blockerPiston('D');     ///< Blocker piston for ring flow control (ADI port D)
inline pros::ADIDigitalOut matchLoaderPiston('B'); ///< Match loader piston for skills loading (ADI port B)
inline pros::ADIDigitalOut parkPiston('A');        ///< Park piston for endgame positioning (ADI port A)
inline pros::ADIDigitalOut intakeLiftPiston('H');  ///< Intake lift piston for height adjustment (ADI port H)

// Piston wrapper objects for state management
inline Piston blocker(&blockerPiston);         ///< Blocker piston controller
inline Piston flip(&flipPiston);               ///< Flip piston controller
inline Piston park(&parkPiston);               ///< Park piston controller
inline Piston matchLoader(&matchLoaderPiston); ///< Match loader piston controller
inline Piston intakeLift(&intakeLiftPiston);   ///< Intake lift piston controller   ///< Intake lift piston controller

// ============================================================================
// Robot Control Objects
// ============================================================================

inline Roller roller(front, middle, intake, back, flip, blocker, intakeLift); ///< Roller system controller

inline Robot robot(roller, matchLoader, master); ///< High-level robot controller with driver input  ///< High-level robot controller with driver input

// ============================================================================
// LemLib Drivetrain Configuration
// ============================================================================

/**
 * @brief LemLib drivetrain configuration
 *
 * Parameters:
 * - Track width: 12 inches (distance between left and right wheels)
 * - Wheel type: NEW_325 (3.25" omni wheels)
 * - RPM: 450 (motor cartridge speed)
 * - Chase power: 2 (used for precise movements)
 */
inline lemlib::Drivetrain LEMLIB_drivetrain(&leftDT, &rightDT,
                                            12,
                                            lemlib::Omniwheel::NEW_325,
                                            450,
                                            2);

/**
 * @brief Vertical tracking wheel configuration
 *
 * Uses 2.75" omni wheel with -0.25" offset from center of rotation
 */
inline lemlib::TrackingWheel LEMLIB_tracking_vertical(&trackingVertical, lemlib::Omniwheel::NEW_275, -0.25);

// ============================================================================
// LemLib Odometry and PID Configuration
// ============================================================================

/**
 * @brief Odometry sensor configuration
 *
 * Uses vertical tracking wheel and IMU for position tracking.
 * Horizontal tracking wheels are not used (nullptr).
 */
inline lemlib::OdomSensors LEMLIB_sensors(&LEMLIB_tracking_vertical, nullptr, nullptr, nullptr, &imu);

/**
 * @brief Lateral (forward/backward) movement PID controller
 *
 * PID Constants:
 * - kP: 6.0 - Proportional gain for position error correction
 * - kI: 0.5 - Integral gain to eliminate steady-state error
 * - kD: 71.0 - Derivative gain for dampening oscillations
 * - Anti-windup: 1.285 - Prevents integral term from growing too large
 *
 * Exit Conditions:
 * - Small error: 1" tolerance, 100ms timeout
 * - Large error: 3" tolerance, 500ms timeout
 * - Slew rate: 10 (limits acceleration)
 */
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

/**
 * @brief Angular (turning) movement PID controller
 *
 * PID Constants:
 * - kP: 3.0 - Proportional gain for heading error correction
 * - kI: 0.94 - Integral gain to eliminate steady-state error
 * - kD: 25.1 - Derivative gain for dampening oscillations
 * - Anti-windup: 1.38734 - Prevents integral term from growing too large
 *
 * Exit Conditions:
 * - Small error: 1° tolerance, 100ms timeout
 * - Large error: 3° tolerance, 500ms timeout
 * - Slew rate: 0 (no acceleration limiting for turns)
 */
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

/**
 * @brief Main chassis controller combining all LemLib components
 *
 * Integrates drivetrain, PID controllers, and odometry sensors for
 * autonomous movement and position tracking.
 */
inline lemlib::Chassis chassis(LEMLIB_drivetrain,         // drivetrain settings
                               LEMLIB_lateral_controller, // lateral PID settings
                               LEMLIB_angular_controller, // angular PID settings
                               LEMLIB_sensors             // odometry sensors
);

#endif