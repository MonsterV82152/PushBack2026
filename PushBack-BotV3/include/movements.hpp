#pragma once

#ifndef MOVEMENTS_HPP
#define MOVEMENTS_HPP

#include "main.h"
#include "piston.hpp"

/**
 * @brief Helper class to bundle robot components
 */
class Helper
{
public:
    /**
     * @brief Constructor for Helper class
     * @param left1 Port number for the first left drive motor
     * @param left2 Port number for the second left drive motor
     * @param right1 Port number for the first right drive motor
     * @param right2 Port number for the second right drive motor
     * @param leftIntake Port number for the left intake motor
     * @param rightIntake Port number for the right intake motor
     * @param leftHook Port number for the left hook motor
     * @param rightHook Port number for the right hook motor
     * @param scoringRotation Reference to the scoring rotation sensor
     * @param scoreLift Reference to the scoring lift piston
     * @param intakePTO Reference to the intake PTO piston
     * @param hookPTO Reference to the hook PTO piston
     */
    Helper(
        std::int8_t left1,
        std::int8_t left2,
        std::int8_t right1,
        std::int8_t right2,
        std::int8_t leftIntake,
        std::int8_t rightIntake,
        std::int8_t leftHook,
        std::int8_t rightHook,
        pros::Rotation &scoringRotation,
        Piston &scoreLift,
        Piston &intakePTO,
        Piston &hookPTO);
    std::int8_t leftIntakeMotorPort;
    std::int8_t rightIntakeMotorPort;
    std::int8_t leftHookMotorPort;
    std::int8_t rightHookMotorPort;
    pros::MotorGroup leftSensorMotor;
    pros::MotorGroup rightSensorMotor;
    pros::Motor leftIntakeMotor;
    pros::Motor rightIntakeMotor;
    pros::Motor leftHookMotor;
    pros::Motor rightHookMotor;
    pros::MotorGroup leftDT;
    pros::MotorGroup rightDT;

    pros::Rotation &scoringRotation;
    Piston &scoreLift;
    Piston &intakePTO;
    Piston &hookPTO;

private:
};

// State definitions
#define ON 1 // PTO is engaged
#define OFF 0 // PTO is disengaged
#define LEAVE 128 // Leave the PTO state unchanged  

/**
 * @brief Struct to represent the state of the PTOs and their speeds
 */
struct PTOState
{
    short intakePTOState;
    short hookPTOState;
    short intakeSpeed;
    short hookSpeed;
};

/**
 * @brief Enum to represent different scoring actions
 */
enum class ScoringAction
{
    DESCOREANDHOLD,
    DESCOREANDRESET,
    /**
     * @brief Score and reset the scoring mechanism
     */
    SCOREANDRESET,
    /**
     * @brief Score and hold the scoring mechanism in position
     */
    SCOREANDHOLD,
    /**
     * @brief Reset the scoring mechanism to its initial position
     */
    RESET,
    /**
     * @brief Hold the scoring mechanism in its current position
     */
    HOLD,
    DEHOLD
};

class Robot
{
public:
    /**
     * @brief Constructor for Robot class
     * @param helper Reference to a Helper object containing robot components
     * @param mcl Reference to a limelib::MCL object for motion control
     * @param controller Reference to a pros::Controller object for user input
     */
    Robot(Helper &helper, limelib::MCL &mcl, pros::Controller &controller);
    /**
     * @brief Initialize the robot's systems
     */
    void init();
    /**
     * @brief Control the robot during teleoperation
     */
    void teleopControl();
    /**
     * @brief Control the intake mechanism
     */
    void intake();
    /**
     * @brief Control the lift mechanism
     */
    void score();
    /**
     * @brief Move the robot based on the given PTO state
     * @param state The desired PTO state
     */
    void moveState(PTOState state);
    /**
     * @brief Move the robot to a specific point
     * @param point The target point to move to
     * @param timeout The maximum time to attempt the movement
     * @param params Additional parameters for the movement
     */
    void moveToPoint(limelib::Point2D point, int timeout, limelib::moveToPointParams params = limelib::moveToPointParams());
    /**
     * @brief Move the robot to a specific pose
     * @param pose The target pose to move to
     */
    void moveToPose(limelib::Pose2D pose, int timeout, limelib::moveToPoseParams params = limelib::moveToPoseParams());
    /**
     * @brief Turn the robot to a specific heading
     * @param heading The target heading in degrees
     */
    void turnToHeading(limelib::real_t heading, int timeout, limelib::turnToHeadingParams params = limelib::turnToHeadingParams());
    /**
     * @brief Turn the robot to face a specific point
     * @param point The target point to face
     */
    void turnToPoint(limelib::Point2D point, int timeout, limelib::turnToHeadingParams params = limelib::turnToHeadingParams());

private:
    void scoringLoop();                          // Main scoring task function
    void setScoringAction(ScoringAction action); // Set the current scoring action

    constexpr static int SCORING_POSITION = 1800;
    constexpr static int DESCORING_POSITION = -1800;


    Helper &helper;
    pros::Controller &master;

    limelib::Chassis chassis;
    limelib::PID lateralPID8;
    limelib::PID angularPID8;
    limelib::PID lateralPID6;
    limelib::PID angularPID6;
    limelib::PID lateralPID4;
    limelib::PID angularPID4;
    limelib::MCL &mcl;

    std::unique_ptr<pros::Task> hookPTOTask;
    std::unique_ptr<pros::Task> intakePTOTask;
    std::unique_ptr<pros::Task> scoringTask;
    std::atomic<bool> hookPTOState;
    std::atomic<bool> intakePTOState;
    std::atomic<ScoringAction> currentScoringAction{ScoringAction::RESET};
    std::atomic<bool> scoringTaskRunning{false};
    limelib::PID hookPID;
    bool intaking = false;
    bool liftState = false;
    bool intakeTaskQueued = false;
    bool hookTaskQueued = false;
    double currentAngle = 0;
};

#endif