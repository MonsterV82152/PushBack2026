#include "limelib/motion/chassis.hpp"

limelib::Chassis::Chassis(Locator &locator, pros::MotorGroup &leftDr, pros::MotorGroup &rightDr, PID &lateralController, PID &velocityController, PID &angularController, TrapezoidalMotionProfile &motionProfile)
    : locator(locator), leftDr(leftDr), rightDr(rightDr), lateralController(lateralController), velocityController(velocityController), angularController(angularController), motionProfile(motionProfile), movementHelper()
{
}

void limelib::Chassis::calibrate()
{
    locator.calibrate();
    angularController.reset();
    lateralController.reset();
    velocityController.reset();
    motionProfile.reset();
    movementHelper.cancel();
}

void limelib::Chassis::cancelAllMovement()
{
    movementHelper.cancel();
}

void limelib::Chassis::moveToPoint(Point2D point, int timeout, moveToPointParams params)
{
    movementHelper.reset(timeout);
    while (!movementHelper.isDone())
    {
        Pose2D currentPose = locator.getPose();
        real_t distance = movementHelper.getDistance(Pose2D{point.x, point.y, 0}, currentPose);
        if (distance <= params.earlyExitRange)
        {
            break;
        }

                pros::delay(10);
    }
}

void limelib::Chassis::moveToPoint(real_t x, real_t y, int timeout, moveToPointParams params)
{
    moveToPoint(Point2D{x, y}, timeout, params);
}

void limelib::Chassis::moveToPose(Pose2D pose, int timeout, moveToPoseParams params)
{
}

void limelib::Chassis::moveToPose(real_t x, real_t y, real_t theta, int timeout, moveToPoseParams params)
{
    moveToPose(Pose2D{x, y, theta}, timeout, params);
}

void limelib::Chassis::turnToHeading(real_t heading, int timeout, turnToHeadingParams params)
{
    // Implementation of turning to a heading
}

void limelib::Chassis::turnToPoint(Point2D point, int timeout, turnToHeadingParams params)
{
}

void limelib::Chassis::turnToPoint(real_t x, real_t y, int timeout, turnToHeadingParams params)
{
    turnToPoint(Point2D{x, y}, timeout, params);
}