#pragma once

#ifndef ROBOT_COMMANDS_HPP
#define ROBOT_COMMANDS_HPP

#include "defines.hpp"
#include "globals.hpp"
#include "misc/sensor_loc.hpp"
#include "commands/api.hpp"
#include "robot_states.hpp"

class JoystickDrive : public Command
{
public:
    JoystickDrive()
    {
        addRequirements({DRIVETRAIN_ID});
    }
    void initialize() override {}
    void execute() override
    {
        double forward = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        double turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        chassis.arcade(forward, turn);
    }
    void end(bool interrupted) override { chassis.arcade(0, 0, false, 0.67); }
    bool isFinished() const override { return false; }
};

class MoveToPoint : public Command
{
public:
    MoveToPoint(float x, float y, int timeout, lemlib::MoveToPointParams params = lemlib::MoveToPointParams())
        : target(x, y), params(params), movementTimeout(timeout)
    {
        this->timeout = timeout;
        addRequirements({DRIVETRAIN_ID});
    }
    void initialize() override
    {
        chassis.moveToPoint(target.x, target.y, movementTimeout, params);
    }
    void execute() override {}
    void end(bool interrupted) override
    {
        if (interrupted)
            chassis.cancelMotion();
    }
    bool isFinished() const override { return false; }

private:
    lemlib::Pose target;
    lemlib::MoveToPointParams params;
    int movementTimeout;
};

class TurnToHeading : public Command
{
public:
    TurnToHeading(float heading, int timeout, lemlib::TurnToHeadingParams params = lemlib::TurnToHeadingParams())
        : target(heading), params(params), movementTimeout(timeout)
    {
        this->timeout = timeout;
        addRequirements({DRIVETRAIN_ID});
    }
    void initialize() override
    {
        chassis.turnToHeading(target, movementTimeout, params);
    }
    void execute() override {}
    void end(bool interrupted) override
    {
        if (interrupted)
            chassis.cancelMotion();
    }
    bool isFinished() const override { return false; }

private:
    float target;
    lemlib::TurnToHeadingParams params;
    int movementTimeout;
};

class TurnToPoint : public Command
{
public:
    TurnToPoint(float x, float y, int timeout, lemlib::TurnToPointParams params = lemlib::TurnToPointParams())
        : target(x, y), params(params), movementTimeout(timeout)
    {
        this->timeout = timeout;
        addRequirements({DRIVETRAIN_ID});
    }
    void initialize() override
    {
        chassis.turnToPoint(target.x, target.y, movementTimeout, params);
    }
    void execute() override {}
    void end(bool interrupted) override
    {
        if (interrupted)
            chassis.cancelMotion();
    }
    bool isFinished() const override { return false; }

private:
    lemlib::Pose target;
    lemlib::TurnToPointParams params;
    int movementTimeout;
};

Command *switchPTOState(bool state);

class TransitionToScore : public Command
{
public:
    TransitionToScore(std::function<double(double)> &scoreFunc)
        : scoreFunction(scoreFunc)
    {
        addRequirements({MANIPULATOR_ID});
    }
    void initialize() override
    {
        RobotStates::targetState = RobotState::SCORING;
        if (scoreFunction(1.0) >= 127)
            scoringPiston.setState(true);
    }
    void execute() override
    {
        double potValue = potentiometer.get_value();
        angle = (double)(potValue - SCORING_DOWN_ANGLE) / (double)(SCORING_UP_ANGLE - SCORING_DOWN_ANGLE);
        angle = std::clamp(angle, 0.0, 1.0);
        if (angle < 0.90)
        {
            RobotStates::currentState = RobotState::TRANSITIONING;
            RobotStates::stickState = RobotState::TRANSITIONING;
            motors.move(scoreFunction(angle));
        }
    }
    void end(bool interrupted) override
    {
        if (!interrupted)
        {
            RobotStates::currentState = RobotState::SCORING;
            RobotStates::stickState = RobotState::SCORING;
        }
    }
    bool isFinished() const override { return angle > 0.90; }

private:
    std::function<double(double)> scoreFunction;
    double angle = 0;
};

class HoldScoring : public Command
{
public:
    HoldScoring(std::function<double(double)> &scoreFunc)
        : scoreFunction(scoreFunc)
    {
        addRequirements({MANIPULATOR_ID});
    }
    void initialize() override
    {
    }
    void execute() override
    {
        double potValue = potentiometer.get_value();
        angle = (double)(potValue - SCORING_DOWN_ANGLE) / (double)(SCORING_UP_ANGLE - SCORING_DOWN_ANGLE);
        angle = std::clamp(angle, 0.0, 1.0);
        if (angle < 0.90)
        {
            motors.move(scoreFunction(angle));
        }
        else
        {
            motors.brake();
        }
    }
    void end(bool interrupted) override
    {
    }
    bool isFinished() const override { return false; }

private:
    std::function<double(double)> scoreFunction;
    double angle = 0;
};

Command *score(std::function<double(double)> &scoreFunc);

class LowerScoring : public Command
{
public:
    LowerScoring()
    {
        addRequirements({MANIPULATOR_ID});
    }
    void initialize() override
    {
        RobotStates::targetState = RobotState::INTAKING;
        scoringPiston.setState(false);
    }
    void execute() override
    {
        int32_t potValue = potentiometer.get_value();
        angle = (double)(potValue - SCORING_DOWN_ANGLE) / (double)(SCORING_UP_ANGLE - SCORING_DOWN_ANGLE);
        angle = std::clamp(angle, 0.0, 1.0);

        RobotStates::currentState = RobotState::TRANSITIONING;
        RobotStates::stickState = RobotState::TRANSITIONING;
        motors.move(-127);
    }
    void end(bool interrupted) override
    {
        if (!interrupted)
        {
            RobotStates::stickState = RobotState::INTAKING;
        }
        motors.brake();
    }
    bool isFinished() const override { return angle < 0.1; }

private:
    double angle = 1;
};

class StartIntake : public Command
{
public:
    StartIntake()
    {
        addRequirements({MANIPULATOR_ID});
    }
    void initialize() override
    {
        RobotStates::currentState = RobotState::INTAKING;
        scoringPiston.setState(false);
        motors.move(127);
    }
    void execute() override
    {
    }
    void end(bool interrupted) override
    {
        motors.brake();
        RobotStates::currentState = RobotState::IDLE;
    }
    bool isFinished() const override { return false; }
};

class StopIntake : public Command
{
public:
    StopIntake()
    {
        addRequirements({MANIPULATOR_ID});
    }
    void initialize() override
    {
        if (RobotStates::currentState == RobotState::INTAKING)
            motors.move(0);
        RobotStates::currentState = RobotState::IDLE;
    }
    void execute() override
    {
    }
    void end(bool interrupted) override
    {
        RobotStates::currentState = RobotState::IDLE;
    }
    bool isFinished() const override { return true; }
};

class ReverseIntake : public Command
{
public:
    ReverseIntake()
    {
        addRequirements({MANIPULATOR_ID});
    }
    void initialize() override
    {
        RobotStates::currentState = RobotState::OUTTAKING;
        scoringPiston.setState(false);
        motors.move(-127);
    }
    void execute() override
    {
    }
    void end(bool interrupted) override
    {
        motors.brake();
        RobotStates::currentState = RobotState::IDLE;
    }
    bool isFinished() const override { return false; }
};

Command *intake();
Command *reverseIntake();
Command *lowerScoring();

#endif