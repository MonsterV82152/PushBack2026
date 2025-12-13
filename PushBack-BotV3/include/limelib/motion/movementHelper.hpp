#pragma once

#ifndef LIMELIB_MOTION_MOVEMENTHELPER_HPP
#define LIMELIB_MOTION_MOVEMENTHELPER_HPP

#include "limelib/types.hpp"
#include "limelib/geometry/pose2d.hpp"
#include "limelib/geometry/point2d.hpp"
#include "pros/rtos.hpp"
#include <atomic>

namespace limelib
{
    class Timer
    {
    public:
        Timer(int milis);
        void update();
        int elapsedTime();
        bool isDone();
        void restart(int milis);

    private:
        int duration;
        int elapsed;
        int startTime;
    };

    class MovementHelper
    {
    public:
        MovementHelper();
        bool isDone();
        int elapsedTime();
        void cancel();
        void reset(int milis);
        real_t getAngleDiff(real_t targetAngle, real_t currentAngle);
        real_t getDistance(const Pose2D &targetPose, const Pose2D &currentPose);
        real_t getDistance(const Point2D &targetPose, const Pose2D &currentPose);
        real_t getDistance(const Pose2D &targetPose, const Point2D &currentPose);
        real_t getDistance(const Point2D &targetPose, const Point2D &currentPose);


    private:
        std::atomic<bool> cancelled;
        Timer timer;
    };
}

#endif