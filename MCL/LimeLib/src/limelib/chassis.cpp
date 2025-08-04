#include "limelib/chassis.hpp"

limelib::Chassis::Chassis(Localization &locator, pros::MotorGroup &leftDr, pros::MotorGroup &rightDr)
    : locator(locator), leftDr(leftDr), rightDr(rightDr) {
        
    }

void limelib::Chassis::calibrate() {
    locator.calibrate();
}

void limelib::Chassis::moveToPoint(Point2D point, int timeout, moveToPointParams params) {
    moveToPoint(point.getX(), point.getY(), timeout, params);
}

void limelib::Chassis::moveToPoint(real_t x, real_t y, int timeout, moveToPointParams params) {
    // Implementation of moving to a point
}

void limelib::Chassis::moveToPose(Pose2D pose, int timeout, moveToPoseParams params) {
    moveToPose(pose.getX(), pose.getY(), pose.getTheta(), timeout, params);
}

void limelib::Chassis::moveToPose(real_t x, real_t y, real_t theta, int timeout, moveToPoseParams params) {
    // Implementation of moving to a pose
}

void limelib::Chassis::turnToHeading(real_t heading, int timeout, turnToHeadingParams params) {
    // Implementation of turning to a heading
}

void limelib::Chassis::turnToPoint(Point2D point, int timeout, turnToHeadingParams params) {
    turnToPoint(point.getX(), point.getY(), timeout, params);
}

void limelib::Chassis::turnToPoint(real_t x, real_t y, int timeout, turnToHeadingParams params) {
    // Implementation of turning to a point
}