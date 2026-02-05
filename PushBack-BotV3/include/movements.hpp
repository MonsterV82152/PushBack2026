#pragma once

#ifndef MOVEMENTS_HPP
#define MOVEMENTS_HPP

#include "main.h"
#include "piston.hpp"
#include "autonomous_selector.hpp"
#include "sensor_loc.hpp"

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
        pros::Distance &intakeDS,
        Piston &scoreLift,
        Piston &matchLoader,
        Piston &descore,
        Piston &intakePTO,
        Piston &hookPTO,
        Piston &intakeLift,
        AutonSelector &autonSelector);
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
    pros::Distance &intakeDS;

    Piston &scoreLift;
    Piston &matchLoader;
    Piston &descore;
    Piston &intakePTO;
    Piston &hookPTO;
    Piston &intakeLift;

    AutonSelector &autonSelector;

private:
};

// State definitions
#define ON 1      // PTO is engaged
#define OFF 0     // PTO is disengaged
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
    LOWGOAL,
    SCORELOW,
    /**
     * @brief Score and reset the scoring mechanism
     */
    SCOREANDRESET,
    /**
     * @brief Score and hold the scoring mechanism in position
     */
    SCOREANDHOLD,

    UNJAM,
    /**
     * @brief Reset the scoring mechanism to its initial position
     */
    RESET,
    // RESET1,
    /**
     * @brief Hold the scoring mechanism in its current position
     */
    HOLD,
    IDLE
};

class Robot
{
public:
    /**
     * @brief Constructor for Robot class
     * @param helper Reference to a Helper object containing robot components
     * @param locator Reference to a limelib::Locator object for motion control
     * @param controller Reference to a pros::Controller object for user input
     */
    Robot(Helper &helper, limelib::Locator &locator, pros::Controller &controller);
    /**
     * @brief Initialize the robot's systems
     */
    void init();
    void debug();
    /**
     * @brief Control the robot during teleoperation
     */
    void teleopControl();
    /**
     * @brief Control the intake mechanism
     */
    void intake(bool on = true);
    void setLow();
    void scoreLow(int maxSpeed = 35);
    /**
     * @brief Control the lift mechanism
     */
    void score(int position = DEFAULT_SCORING_POSITION, int maxSpeed = 127, bool approach = false);
    /**
     * @brief Control the lift mechanism
     * @param up True to lift up, false to lower
     */
    void lift(bool up = true);
    /**
     * @brief Control the match loader mechanism
     * @param load True to engage the match loader, false to disengage
     */
    void matchLoad(bool load = true);
    /**
     * @brief Control the descore mechanism
     * @param descoring True to engage descore, false to disengage
     */
    void descore(bool descoring = true);
    void lowerIntake();
    void raiseIntake();
    void disabled();
    void reset();
    /**
     * @brief Set the robot's pose
     * @param x The desired x-coordinate of the pose
     * @param y The desired y-coordinate of the pose
     * @param theta The desired orientation (in degrees) of the pose
     */
    void setPose(limelib::real_t x, limelib::real_t y, limelib::real_t theta);
    /**
     * @brief Wait until the current movement is complete
     */
    void waitUntilDone();
    /**
     * @brief Move the robot based on the given PTO state
     * @param state The desired PTO state
     */
    void moveState(PTOState state);
    /**
     * @brief Move the robot to a specific point
     * @param x The target x-coordinate
     * @param y The target y-coordinate
     * @param timeout The maximum time to attempt the movement
     * @param params Additional parameters for the movement
     */
    void moveToPoint(limelib::real_t x, limelib::real_t y, int timeout, limelib::moveToPointParams params = limelib::moveToPointParams());
    /**
     * @brief Turn the robot to a specific heading
     * @param heading The target heading in degrees
     */
    void turnToHeading(limelib::real_t heading, int timeout, limelib::turnToHeadingParams params = limelib::turnToHeadingParams());
    /**
     * @brief Turn the robot to face a specific point
     * @param x The target x-coordinate
     * @param y The target y-coordinate
     */
    void turnToPoint(limelib::real_t x, limelib::real_t y, int timeout, limelib::turnToHeadingParams params = limelib::turnToHeadingParams());
    void setScoringAction(ScoringAction action); // Set the current scoring action

private:
    void scoringLoop();                    // Main scoring task function
    constexpr static int FEEDFORWARD = 10; // Angle to unjam the scoring mechanism
    constexpr static int DEFAULT_SCORING_POSITION = 1800;
    constexpr static int DEFAULT_DESCORING_POSITION = -1750;
    constexpr static int LOWGOAL_POSITION = 1730;
    constexpr static int SCORELOW_POSITION = 50;
    constexpr static int TURN_THRESHOLD = 10;         // Threshold for in-place turning
    constexpr static double TURN_SENSITIVITY = 0.012; // Adjust for desired turning responsiveness
    int scoringPosition = DEFAULT_SCORING_POSITION;
    int descoringPosition = DEFAULT_DESCORING_POSITION;
    int maxSpeed = 127;
    ScoringAction lastState = ScoringAction::IDLE;
    Helper &helper;
    pros::Controller &master;

    limelib::Chassis chassis;
    limelib::PID linearPID8;
    limelib::PID angularPID8;
    limelib::PID linearPID6;
    limelib::PID angularPID6;
    limelib::PID linearPID4;
    limelib::PID angularPID4;
    limelib::Locator &locator;

    std::unique_ptr<pros::Task> hookPTOTask;
    std::unique_ptr<pros::Task> intakePTOTask;
    std::unique_ptr<pros::Task> scoringTask;
    std::unique_ptr<pros::Task> descoreTask;
    std::atomic<bool> hookPTOState;
    std::atomic<bool> intakePTOState;
    std::atomic<bool> intakeTaskQueued{false};
    std::atomic<bool> hookTaskQueued{false};
    std::atomic<bool> fullSpeed{false};
    std::atomic<bool> descoring{false};
    std::atomic<bool> lowGoalPosition{false};
    std::atomic<bool> approaching{false};
    std::atomic<ScoringAction> currentScoringAction{ScoringAction::IDLE};
    std::atomic<bool> scoringTaskRunning{false};
    std::atomic<bool> parking{false};
    limelib::PID hookPID;
    bool intaking = false;
    bool liftState = false;
    int motorCount = 4;
};

#endif