#pragma once

#ifndef LIMELIB_POSE2D_HPP
#define LIMELIB_POSE2D_HPP
#include "limelib/types.hpp"

namespace limelib
{
    struct Pose2D
    {
        real_t x;
        real_t y;
        real_t theta;
        Pose2D(real_t x = 0, real_t y = 0, real_t theta = 0) : x(x), y(y), theta(theta) {}
        Pose2D toDegrees() const
        {
            return Pose2D(x, y, theta * 180 / M_PI);
        }
        Pose2D toRadians() const
        {
            return Pose2D(x, y, theta * M_PI / 180);
        }
    };
}

#endif // LIMELIB_POSE2D_HPP