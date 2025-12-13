#include "limelib/motion/chassis.hpp"

limelib::Chassis::Chassis(Locator &locator, pros::MotorGroup &leftDr, pros::MotorGroup &rightDr, PID &lateralController, PID &velocityController, PID &angularController, TrapezoidalMotionProfile &motionProfile)
    : locator(locator), leftDr(leftDr), rightDr(rightDr), lateralController(lateralController), velocityController(velocityController), angularController(angularController), motionProfile(motionProfile), movementTask([]() {}, "LIMELIB_MOVEMENT"), movementHelper()
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
    movementTask.suspend();
}

void limelib::Chassis::cancelAllMovement()
{
    movementHelper.cancel();
}

void limelib::Chassis::setPose(Pose2D pose)
{
    locator.setPose(pose);
}

void limelib::Chassis::setPose(real_t x, real_t y, real_t theta)
{
    locator.setPose(Pose2D{x, y, theta});
}

void limelib::Chassis::moveToPoint(Point2D point, int timeout, moveToPointParams params)
{
    while (!movementHelper.isDone())
    {
        pros::delay(10);
    }
    movementTask.remove();
    movementTask.create([this, point, timeout, params]()
                        { moveToPointTask(point, timeout, params); }, "LIMELIB_MOVE_TO_POINT");
}
void limelib::Chassis::moveToPointTask(Point2D point, int timeout, moveToPointParams params)
{
    movementHelper.reset(timeout);
    motionProfile.reset();
    motionProfile.setMaxSpeed(params.maxSpeed);
    Pose2D currentPose = locator.getPose();
    real_t totalDistance = movementHelper.getDistance(point, currentPose);
    motionProfile.generateProfile(totalDistance);
    while (!movementHelper.isDone())
    {
        currentPose = locator.getPose();
        std::cout << "Current Pose: (" << currentPose.x << ", " << currentPose.y << ", " << currentPose.theta << ")\n";
        real_t distance = movementHelper.getDistance(point, currentPose);
        std::cout << "Distance to Point: " << distance << "\n";
        if (distance <= params.earlyExitRange)
        {
            break;
        }
        real_t targetVelocity = motionProfile.getVelocity(movementHelper.elapsedTime() / 1000.0);
        real_t heading = params.forwards ? std::atan2(point.x - currentPose.x, point.y - currentPose.y) * 180 / M_PI : std::atan2(currentPose.x - point.x, currentPose.y - point.y) * 180 / M_PI;
        std::cout << "Target Heading: " << heading << "\n";
        std::cout << "Target Velocity: " << targetVelocity << ", Current Velocity: " << locator.getVelocity().linear << "\n";
        real_t velocityOutput = lateralController.update(motionProfile.getPosition(movementHelper.elapsedTime() / 1000.0) - (totalDistance - distance)) + targetVelocity;
        real_t angularError = movementHelper.getAngleDiff(heading, currentPose.theta);
        std::cout << "Angular Error: " << angularError << "\n";
        real_t angularOutput = angularController.update(angularError);
        // clamp velocity outputs to minSpeed and maxSpeed
        if (std::abs(velocityOutput) < params.minSpeed)
        {
            velocityOutput = (velocityOutput >= 0 ? 1 : -1) * params.minSpeed;
        }
        if (std::abs(velocityOutput) > params.maxSpeed)
        {
            velocityOutput = (velocityOutput >= 0 ? 1 : -1) * params.maxSpeed;
        }
        std::cout << "Velocity Output: " << velocityOutput << "\n";
        real_t lateralOutput = velocityController.update(velocityOutput - locator.getVelocity().linear)*(1-abs(angularError)/180);
        std::cout << "Lateral Output: " << lateralOutput << ", Angular Output: " << angularOutput << "\n";
        leftDr.move_voltage(static_cast<int16_t>(lateralOutput - angularOutput) * 1000);
        rightDr.move_voltage(static_cast<int16_t>(lateralOutput + angularOutput) * 1000);

        pros::delay(10);
    }
    std::cout << "MoveToPoint complete. Final error: " << movementHelper.getDistance(point, locator.getPose()) << " inches\n";
    movementHelper.cancel();
    leftDr.brake();
    rightDr.brake();
}

void limelib::Chassis::moveToPoint(real_t x, real_t y, int timeout, moveToPointParams params)
{
    moveToPoint(Point2D{x, y}, timeout, params);
}

void limelib::Chassis::turnToHeading(real_t heading, int timeout, turnToHeadingParams params)
{
    while (!movementHelper.isDone())
    {
        pros::delay(10);
    }
    movementTask.remove();
    movementTask.create([this, heading, timeout, params]()
                        { turnToHeadingTask(heading, timeout, params); });
}

void limelib::Chassis::turnToHeadingTask(real_t heading, int timeout, turnToHeadingParams params)
{
    // Stuff
}

void limelib::Chassis::turnToPoint(Point2D point, int timeout, turnToHeadingParams params)
{
    while (!movementHelper.isDone())
    {
        pros::delay(10);
    }
    movementTask.remove();
    Pose2D currentPose = locator.getPose();
    real_t targetHeading = std::atan2(point.y - currentPose.y, point.x - currentPose.x) * 180 / M_PI;
    movementTask.create([this, targetHeading, timeout, params]()
                        { turnToHeadingTask(targetHeading, timeout, params); });
}

void limelib::Chassis::turnToPoint(real_t x, real_t y, int timeout, turnToHeadingParams params)
{
    turnToPoint(Point2D{x, y}, timeout, params);
}