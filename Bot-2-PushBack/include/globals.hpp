/**
 * @file globals.hpp
 * @brief Global constants, variables, and controller definitions
 *
 * This file contains global definitions used throughout the codebase, including:
 * - Mathematical constants
 * - Robot configuration flags (team color, color sorting, etc.)
 * - Controller objects for driver and partner
 * - Controller button aliases for improved readability
 * - Motor brake mode constants
 *
 * These globals provide consistent access to shared state and configuration
 * across all robot subsystems.
 */

#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include "main.h"

// ==============================
// Preprocessor Macros
// ==============================

#define elif else if ///< Shorthand for else if statements

// ==============================
// Mathematical Constants
// ==============================

constexpr double PI = 3.141592; ///< Pi constant for geometric calculations

// ==============================
// Robot Configuration Variables
// ==============================

inline bool isRedTeam = true;       ///< Team alliance color (true = red, false = blue)
inline bool colourSortOn = false;   ///< Enable/disable automatic color sorting of rings
inline bool brakeChassis = false;   ///< Enable/disable chassis brake mode during driver control
inline double hue = 0;              ///< Current hue value for color detection
inline bool programmerMode = false; ///< Enable programmer mode for debugging features

// ==============================
// Color Sensor Thresholds
// ==============================

inline double redMax = 20;   ///< Maximum hue value for red ring detection
inline double redMin = 350;  ///< Minimum hue value for red ring detection
inline double blueMax = 240; ///< Maximum hue value for blue ring detection
inline double blueMin = 170; ///< Minimum hue value for blue ring detection

// ==============================
// Controller Objects
// ==============================

inline pros::Controller master(pros::E_CONTROLLER_MASTER); ///< Primary driver controller
inline pros::Controller slave(pros::E_CONTROLLER_PARTNER); ///< Partner/secondary controller

// ==============================
// Controller Button Aliases
// ==============================

/**
 * @namespace buttons
 * @brief Controller button aliases for improved code readability
 *
 * Provides short, convenient names for PROS controller button constants.
 * Use these instead of the verbose pros::E_CONTROLLER_DIGITAL_* names.
 */
namespace buttons
{
    constexpr auto R1 = pros::E_CONTROLLER_DIGITAL_R1;       ///< Right shoulder button 1
    constexpr auto R2 = pros::E_CONTROLLER_DIGITAL_R2;       ///< Right shoulder button 2
    constexpr auto L1 = pros::E_CONTROLLER_DIGITAL_L1;       ///< Left shoulder button 1
    constexpr auto L2 = pros::E_CONTROLLER_DIGITAL_L2;       ///< Left shoulder button 2
    constexpr auto A = pros::E_CONTROLLER_DIGITAL_A;         ///< A button (bottom face button)
    constexpr auto B = pros::E_CONTROLLER_DIGITAL_B;         ///< B button (right face button)
    constexpr auto Y = pros::E_CONTROLLER_DIGITAL_Y;         ///< Y button (top face button)
    constexpr auto X = pros::E_CONTROLLER_DIGITAL_X;         ///< X button (left face button)
    constexpr auto UP = pros::E_CONTROLLER_DIGITAL_UP;       ///< D-pad up
    constexpr auto DOWN = pros::E_CONTROLLER_DIGITAL_DOWN;   ///< D-pad down
    constexpr auto LEFT = pros::E_CONTROLLER_DIGITAL_LEFT;   ///< D-pad left
    constexpr auto RIGHT = pros::E_CONTROLLER_DIGITAL_RIGHT; ///< D-pad right
}

// ==============================
// Motor Brake Mode Constants
// ==============================

const pros::motor_brake_mode_e_t brake = pros::E_MOTOR_BRAKE_BRAKE; ///< Active braking (fast stop)
const pros::motor_brake_mode_e_t coast = pros::E_MOTOR_BRAKE_COAST; ///< Coast to stop (no resistance)
const pros::motor_brake_mode_e_t hold = pros::E_MOTOR_BRAKE_HOLD;   ///< Hold position (highest resistance)

#endif