#pragma once

#ifndef MOVEMENTS_HPP
#define MOVEMENTS_HPP

#include "main.h"
#include "piston.hpp"

class Helper
{
public:
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
    pros::Motor left1;
    pros::Motor left2;
    pros::Motor right1;
    pros::Motor right2;
    pros::Motor leftIntakeMotor;
    pros::Motor rightIntakeMotor;
    pros::Motor leftHookMotor;
    pros::Motor rightHookMotor;
    pros::MotorGroup leftDT4;
    pros::MotorGroup rightDT4;
    pros::MotorGroup leftDT3Intake;
    pros::MotorGroup rightDT3Intake;
    pros::MotorGroup leftDT3Hook;
    pros::MotorGroup rightDT3Hook;
    pros::MotorGroup leftDT2;
    pros::MotorGroup rightDT2;

    pros::Rotation &scoringRotation;
    Piston &scoreLift;
    Piston &intakePTO;
    Piston &hookPTO;

private:
};

#define ON 1
#define OFF 0
#define LEAVE 128

struct PTOState
{
    short intakePTOState;
    short hookPTOState;
    short intakeSpeed;
    short hookSpeed;
};

enum class ScoringAction
{
    SCOREANDRESET,
    SCOREANDHOLD,
    RESET,
    HOLD
};

class Robot
{
public:
    Robot(Helper &helper, limelib::MCL &mcl, pros::Controller &controller);
    void init();
    void teleopControl();
    void intake();
    void score();
    void moveState(PTOState state);
    void moveToPoint(limelib::Point2D point, int timeout, limelib::moveToPointParams params = limelib::moveToPointParams());
    void moveToPose(limelib::Pose2D pose, int timeout, limelib::moveToPoseParams params = limelib::moveToPoseParams());
    void turnToHeading(limelib::real_t heading, int timeout, limelib::turnToHeadingParams params = limelib::turnToHeadingParams());
    void turnToPoint(limelib::Point2D point, int timeout, limelib::turnToHeadingParams params = limelib::turnToHeadingParams());

private:
    void startScoring();
    void stopScoring();
    void scoringLoop();                          // Main scoring task function
    void setScoringAction(ScoringAction action); // Set the current scoring action

    constexpr static int SCORING_POSITION = 1800;

    Helper &helper;
    pros::Controller &master;

    limelib::Chassis chassis8;
    limelib::Chassis chassis6Intake;
    limelib::Chassis chassis6Hook;
    limelib::Chassis chassis4;
    limelib::PID lateralPID8;
    limelib::PID velocityPID8;
    limelib::PID angularPID8;
    limelib::PID lateralPID6;
    limelib::PID velocityPID6;
    limelib::PID angularPID6;
    limelib::PID lateralPID4;
    limelib::PID velocityPID4;
    limelib::PID angularPID4;
    limelib::TrapezoidalMotionProfile motionProfile8;
    limelib::TrapezoidalMotionProfile motionProfile6;
    limelib::TrapezoidalMotionProfile motionProfile4;
    limelib::MCL &mcl;

    pros::Task hookPTOTask;
    pros::Task intakePTOTask;
    pros::Task scoringTask;
    std::atomic<bool> hookPTOState;
    std::atomic<bool> intakePTOState;
    std::atomic<ScoringAction> currentScoringAction{ScoringAction::RESET};
    std::atomic<bool> scoringTaskRunning{false};
    limelib::PID hookPID;
    bool intaking = false;
    bool liftState = false;
    double currentAngle = 0;
};

#endif