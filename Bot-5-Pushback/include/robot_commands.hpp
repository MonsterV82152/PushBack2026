#pragma once

#ifndef ROBOT_COMMANDS_HPP
#define ROBOT_COMMANDS_HPP

#include "defines.hpp"
#include "sensor_loc.hpp"
#include "commands/api.hpp"

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
    void end() override { chassis.arcade(0, 0); }
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
    void initialize() override {
        chassis.moveToPoint(target.x, target.y, movementTimeout, params);
    }
    void execute() override {}
    void end() override {
        chassis.cancelMotion();
    }
    bool isFinished() const override { return false; }

private:
    lemlib::Pose target;
    lemlib::MoveToPointParams params;
    int movementTimeout;
};

#endif