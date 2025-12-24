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

using namespace limelib;

// 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 21

// ==============================
// Autonomous Selector
// ==============================

inline AutonSelector autonSelect; ///< Autonomous selector for pre-match routine selection

// ==============================
// Drivetrain Motors
// ==============================

inline pros::Imu imu(13);                            ///< Inertial sensor for heading/orientation (port 13)
inline pros::MotorGroup leftDriveMotors({-15, -14}); ///< Left drivetrain motors (ports 14 and 15)
inline pros::MotorGroup rightDriveMotors({19, 20});  ///< Right drivetrain

// ==============================
// Roller System Motors
// ==============================

// ==============================
// Sensors - Distance and Optical
// ==============================
inline pros::Rotation scoringRotation(-7); ///< Rotation sensor for scoring mechanism (port 7)
inline pros::Rotation horizontalTrackingWheel(8);

// ==============================
// Localization Distance Sensors
// ==============================

inline pros::Distance leftDS(1);
inline pros::Distance rightDS(2);
inline pros::Distance frontDS(3);
inline pros::Distance backDS(4);

inline std::vector<MCLDistance> mclDistanceSensors = {
    MCLDistance(leftDS, Pose2D{1, 2, 3}),
    MCLDistance(rightDS, Pose2D{1, 2, 3}),
    MCLDistance(backDS, Pose2D{1, 2, 3}),
    MCLDistance(frontDS, Pose2D{1, 2, 3})};

// ==============================
// Tracking Wheels
// ==============================

inline limelib::TrackingWheel horizontalTW(&horizontalTrackingWheel, 2, 0);          ///< Horizontal tracking wheel
inline limelib::TrackingWheel verticalTW(&leftDriveMotors, &rightDriveMotors, 3.25); ///< Vertical tracking wheel using drivetrain encoders

// ==============================
// Pneumatic Pistons (ADI Ports)
// ==============================

inline pros::ADIDigitalOut intakePTOPiston('A');   ///< Piston on ADI port A
inline pros::ADIDigitalOut hookPTOPiston('B');     ///< Piston on ADI port B
inline pros::ADIDigitalOut scoreLiftPiston('C');   ///< Piston on ADI port C
inline pros::ADIDigitalOut matchLoaderPiston('D'); ///< Piston on ADI port D
inline pros::ADIDigitalOut descorePiston('E');     ///< Piston on ADI port E

// Piston wrapper objects for state management
inline Piston intakePTO(&intakePTOPiston);     ///< Intake piston object
inline Piston hookPTO(&hookPTOPiston);         ///< Hook piston object
inline Piston scoreLift(&scoreLiftPiston);     ///< Scoring lift piston object
inline Piston matchLoader(&matchLoaderPiston); ///< Match loader piston object
inline Piston descore(&descorePiston);         ///< Descore piston object

// ==============================
// Robot Control Objects
// ==============================

inline std::vector<std::shared_ptr<Object2D>> obstacles = {
    std::make_shared<Circle2D>(67.5f, 48.0f, 4.17f),
    std::make_shared<Circle2D>(-67.5f, 48.0f, 4.17f),
    std::make_shared<Circle2D>(67.5f, -48.0f, 4.17f),
    std::make_shared<Circle2D>(-67.5f, -48.0f, 4.17f),
    std::make_shared<Line2D>(Point2D(22.25f, 45.6f), Point2D(20.8f, 47.1f)),
    std::make_shared<Line2D>(Point2D(22.25f, 48.6f), Point2D(20.8f, 47.1f)),
    std::make_shared<Line2D>(Point2D(-22.25f, 45.6f), Point2D(-20.8f, 47.1f)),
    std::make_shared<Line2D>(Point2D(-22.25f, 48.6f), Point2D(-20.8f, 47.1f)),
    std::make_shared<Line2D>(Point2D(22.25f, -45.6f), Point2D(20.8f, -47.1f)),
    std::make_shared<Line2D>(Point2D(22.25f, -48.6f), Point2D(20.8f, -47.1f)),
    std::make_shared<Line2D>(Point2D(-22.25f, -45.6f), Point2D(-20.8f, -47.1f)),
    std::make_shared<Line2D>(Point2D(-22.25f, -48.6f), Point2D(-20.8f, -47.1f)),
    std::make_shared<Line2D>(Point2D(-2.9f, 0.5f), Point2D(0.5f, 2.9f)),
    std::make_shared<Line2D>(Point2D(-0.5f, -2.9f), Point2D(2.9f, -0.5f)),
};
inline Field2D field(144.0f, 144.0f, obstacles);

inline MCL mcl(nullptr, nullptr, imu, mclDistanceSensors, field, 200, 10, 10, false, 10, false);                                ///< LemLib MCL object for odometry and localization
inline Helper helper(-15, -14, 20, 19, 16, -18, 13, -17, scoringRotation, scoreLift, matchLoader, descore, intakePTO, hookPTO); ///< Helper object for robot mechanisms
inline Robot robot(helper, mcl, master);                                                                                        ///< Main robot control object

#endif