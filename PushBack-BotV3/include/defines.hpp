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
#include "sensor_loc.hpp"

using namespace limelib;

// 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 21

// ==============================
// Autonomous Selector
// ==============================

inline AutonSelector autonSelect; ///< Autonomous selector for pre-match routine selection

// ==============================
// Drivetrain Motors
// ==============================

inline pros::Imu imu(12);                                                    ///< Inertial sensor for heading/orientation (port 13)
inline pros::MotorGroup leftDriveMotors({-15, -14}, pros::MotorGears::blue); ///< Left drivetrain motors (ports 14 and 15)
inline pros::MotorGroup rightDriveMotors({19, 20}, pros::MotorGears::blue);  ///< Right drivetrain

// ==============================
// Roller System Motors
// ==============================

// ==============================
// Sensors - Distance and Optical
// ==============================
inline pros::Rotation scoringRotation(-8); ///< Rotation sensor for scoring mechanism (port 7)
inline pros::Rotation horizontalTrackingWheel(11);
inline pros::Distance intakeDS(3);

// ==============================
// Localization Distance Sensors
// ==============================

inline pros::Distance leftDS(2);
inline pros::Distance rightDS(9);
inline pros::Distance frontDS(1);
inline pros::Distance backDS(10);

inline MCLDistance leftDistanceSensor(leftDS, Pose2D{-4.5, 4.5, -90});
inline MCLDistance rightDistanceSensor(rightDS, Pose2D{4.5, 4.5, 90});
inline MCLDistance backDistanceSensor(backDS, Pose2D{5.5, 3, 180});
inline MCLDistance frontDistanceSensor(frontDS, Pose2D{-4.25, 7, 0});

inline std::vector<MCLDistance> mclDistanceSensors = {
    leftDistanceSensor,
    rightDistanceSensor,
    backDistanceSensor,
    frontDistanceSensor};

// ==============================
// Tracking Wheels
// ==============================

inline limelib::TrackingWheel horizontalTW(&horizontalTrackingWheel, 2, 0);                ///< Horizontal tracking wheel
inline limelib::TrackingWheel verticalTW(&leftDriveMotors, &rightDriveMotors, 3.25, 0.75); ///< Vertical tracking wheel using drivetrain encoders

// ==============================
// Pneumatic Pistons (ADI Ports)
// ==============================

inline pros::ADIDigitalOut intakePTOPiston('C');   ///< Piston on ADI port A
inline pros::ADIDigitalOut hookPTOPiston('D');     ///< Piston on ADI port B
inline pros::ADIDigitalOut scoreLiftPiston('E');   ///< Piston on ADI port C
inline pros::ADIDigitalOut matchLoaderPiston('A'); ///< Piston on ADI port D
inline pros::ADIDigitalOut descoreUpPiston('B');   ///< Piston on ADI port E
inline pros::ADIDigitalOut intakeLiftPiston('G');  ///< Piston on ADI port F
inline pros::ADIDigitalOut descoreDownPiston('H'); ///< Piston on ADI port G

// Piston wrapper objects for state management
inline Piston intakePTO(&intakePTOPiston);                   ///< Intake piston object
inline Piston hookPTO(&hookPTOPiston);                       ///< Hook piston object
inline Piston scoreLift(&scoreLiftPiston);                   ///< Scoring lift piston object
inline Piston matchLoader(&matchLoaderPiston);               ///< Match loader piston object
inline Piston descore(&descoreUpPiston, &descoreDownPiston); ///< Descore piston object
inline Piston intakeLift(&intakeLiftPiston);                 ///< Intake lift piston object
// inline Piston intakePark(&intakeParkPiston);   ///< Intake park piston object

// ==============================
// Robot Control Objects
// ==============================
inline std::vector<std::shared_ptr<limelib::Object2D>> fieldObstacles = {};
inline Field2D field(140.5f, 140.5f, fieldObstacles); ///< LemLib Field2D object representing the robot's environment

// inline MCL locator(&verticalTW, &horizontalTW, imu, mclDistanceSensors, field, 500, 0.25, false, -1); ///< LemLib MCL object for odometry and localization
inline Odometry locator(&verticalTW, &horizontalTW, imu);                                                                                                          ///< LemLib Odometry object for basic odometry
inline Helper helper(-15, -14, 20, 19, 16, -18, 13, -17, scoringRotation, intakeDS, scoreLift, matchLoader, descore, intakePTO, hookPTO, intakeLift, autonSelect); ///< Helper object for robot mechanisms
inline Robot robot(helper, locator, master);                                                                                                                       ///< Main robot control object

#endif