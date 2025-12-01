#include "limelib/motion/movementHelper.hpp"

limelib::Timer::Timer(int milis) : duration(milis), elapsed(0), startTime(pros::millis()) {}

void limelib::Timer::update()
{
    elapsed = pros::millis() - startTime;
}
int limelib::Timer::elapsedTime()
{
    update();
    return elapsed;
}
bool limelib::Timer::isDone()
{
    update();
    return elapsed >= duration;
}
void limelib::Timer::restart(int milis)
{
    duration = milis;
    startTime = pros::millis();
    elapsed = 0;
}

limelib::MovementHelper::MovementHelper() : cancelled(false), timer(0) {}
bool limelib::MovementHelper::isDone()
{
    return timer.isDone() || cancelled.load();
}
void limelib::MovementHelper::cancel()
{
    cancelled.store(true);
}
void limelib::MovementHelper::reset(int milis)
{
    cancelled.store(false);
    timer.restart(milis);
}
limelib::real_t limelib::MovementHelper::getAngleDiff(real_t targetAngle, real_t currentAngle)
{
    real_t diff = targetAngle - currentAngle;
    while (diff > 180)
        diff -= 360;
    while (diff < -180)
        diff += 360;
    return diff;
}

limelib::real_t limelib::MovementHelper::getDistance(const Pose2D &targetPose, const Pose2D &currentPose)
{
    real_t dx = targetPose.x - currentPose.x;
    real_t dy = targetPose.y - currentPose.y;
    return std::sqrt(dx * dx + dy * dy);
}