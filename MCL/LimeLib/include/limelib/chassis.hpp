#pragma once

#ifndef LIMELIB_CHASSIS_HPP
#define LIMELIB_CHASSIS_HPP


#include "limelib/types.hpp"
#include "limelib/locator.hpp"
#include "limelib/pid.hpp"
#include "pros/motor_group.hpp"


namespace limelib
{
    struct moveToPointParams
    {
        bool forwards = true;
        real_t maxSpeed = 127;
        real_t minSpeed = 0;
        real_t earlyExitRange = 0.1;
    };

    struct moveToPoseParams
    {
        bool lateralForwards = true;
        bool angularForwards = true;
        real_t maxSpeed = 127;
        real_t minSpeed = 0;
        real_t earlyExitRange = 0.1;
    };
    struct turnToHeadingParams
    {
        bool forwards = true;
        real_t maxSpeed = 127;
        real_t minSpeed = 0;
        real_t earlyExitRange = 0.1;
    };
    class Chassis
    {
    public:
        Chassis(Locator &locator, pros::MotorGroup &leftDr, pros::MotorGroup &rightDr, PID &lateralController, PID &angularController);
        void calibrate();
        void moveToPoint(Point2D point, int timeout, moveToPointParams params = moveToPointParams());
        void moveToPoint(real_t x, real_t y, int timeout, moveToPointParams params = moveToPointParams());
        void moveToPose(Pose2D pose, int timeout, moveToPoseParams params = moveToPoseParams());
        void moveToPose(real_t x, real_t y, real_t theta, int timeout, moveToPoseParams params = moveToPoseParams());
        void turnToHeading(real_t heading, int timeout, turnToHeadingParams params = turnToHeadingParams());
        void turnToPoint(Point2D point, int timeout, turnToHeadingParams params = turnToHeadingParams());
        void turnToPoint(real_t x, real_t y, int timeout, turnToHeadingParams params = turnToHeadingParams());

    private:
        Locator &locator;
        pros::MotorGroup &leftDr;
        pros::MotorGroup &rightDr;
        PID &lateralController;
        PID &angularController;

        
    };
}

#endif