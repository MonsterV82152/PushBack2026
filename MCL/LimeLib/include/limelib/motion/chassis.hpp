#pragma once

#ifndef LIMELIB_CHASSIS_HPP
#define LIMELIB_CHASSIS_HPP

#include "limelib/types.hpp"
#include "limelib/locator.hpp"
#include "limelib/motion/pid.hpp"
#include "limelib/motion/trapezoidalMotionProfiler.hpp"
#include "limelib/motion/movementHelper.hpp"
#include "pros/motor_group.hpp"

namespace limelib
{
    /**
     * Parameters for moveToPoint function
     */
    struct moveToPointParams
    {
        bool forwards = true;
        real_t maxSpeed = 127;
        real_t minSpeed = 0;
        real_t earlyExitRange = 0.1;
    };
    /**
     * Parameters for moveToPose function
     */
    struct moveToPoseParams
    {
        bool lateralForwards = true;
        bool angularForwards = true;
        real_t maxSpeed = 127;
        real_t minSpeed = 0;
        real_t earlyExitRange = 0.1;
    };
    /**
     * Parameters for turnToHeading function
     */
    struct turnToHeadingParams
    {
        bool forwards = true;
        real_t maxSpeed = 127;
        real_t minSpeed = 0;
        real_t earlyExitRange = 0.1;
    };
    /**
     * Class representing a robot chassis
     */
    class Chassis
    {
    public:
        /**
         * Constructor for Chassis
         * @param locator Reference to a Locator object for localization
         * @param leftDr Reference to the left motor group
         * @param rightDr Reference to the right motor group
         * @param lateralController Reference to the lateral PID controller
         * @param velocityController Reference to the velocity PID controller
         * @param angularController Reference to the angular PID controller
         * @param motionProfile Reference to the trapezoidal motion profiler
         */
        Chassis(Locator &locator, pros::MotorGroup &leftDr, pros::MotorGroup &rightDr, PID &lateralController, PID &velocityController, PID &angularController, TrapezoidalMotionProfile &motionProfile);
        void calibrate();
        void cancelAllMovement();
        void moveToPoint(Point2D point, int timeout, moveToPointParams params = moveToPointParams());
        void moveToPoint(real_t x, real_t y, int timeout, moveToPointParams params = moveToPointParams());
        void moveToPose(Pose2D pose, int timeout, moveToPoseParams params = moveToPoseParams());
        void moveToPose(real_t x, real_t y, real_t theta, int timeout, moveToPoseParams params = moveToPoseParams());
        void turnToHeading(real_t heading, int timeout, turnToHeadingParams params = turnToHeadingParams());
        void turnToPoint(Point2D point, int timeout, turnToHeadingParams params = turnToHeadingParams());
        void turnToPoint(real_t x, real_t y, int timeout, turnToHeadingParams params = turnToHeadingParams());

    private:
        Locator &locator;
        MovementHelper movementHelper;
        pros::MotorGroup &leftDr;
        pros::MotorGroup &rightDr;
        PID &lateralController;
        PID &velocityController;
        PID &angularController;
        bool isMoving = false;
        bool motionQueued = false;
        TrapezoidalMotionProfile &motionProfile;
    };
}

#endif