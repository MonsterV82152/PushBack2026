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
#include "autonomous_selector.hpp"
#include "movements.hpp"
#include "piston.hpp"

// ==============================
// Autonomous Selector
// ==============================

inline AutonSelector autonSelect; ///< Autonomous selector for pre-match routine selection

// ==============================
// Drivetrain Motors
// ==============================

inline pros::MotorGroup leftDT({-1, -2}); ///< Left drivetrain motor group (ports 1, 2)
inline pros::MotorGroup rightDT({8, 9});  ///< Right drivetrain motor group (ports 8, 9)
inline pros::MotorGroup rightDT2({8, 9});  ///< Right drivetrain motor group (ports 8, 9)

inline pros::Imu imu(13);                 ///< Inertial sensor for heading/orientation (port 13)

// ==============================
// Roller System Motors
// ==============================
inline pros::Motor leftRollerMotor(-3); ///< Left roller motor (port 5
inline pros::Motor rightRollerMotor(4); ///< Right roller motor (port 6)

inline pros::Motor leftFishMotor(-5); ///< Left fish mechanism motor (port 5)
inline pros::Motor rightFishMotor(6); ///< Right fish mechanism motor (port 6)

// ==============================
// Sensors - Distance and Optical
// ==============================

// ==============================
// Localization Distance Sensors
// ==============================

// ==============================
// Tracking Wheels
// ==============================

// ==============================
// Pneumatic Pistons (ADI Ports)
// ==============================

inline pros::ADIDigitalOut intakePTOPiston('A'); ///< Piston on ADI port A

// Piston wrapper objects for state management
inline Piston intakePTO(&intakePTOPiston); ///< Intake piston object

// ==============================
// Robot Control Objects
// ==============================

#endif