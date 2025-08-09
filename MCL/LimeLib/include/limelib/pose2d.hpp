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
    };
}

#endif // LIMELIB_POSE2D_HPP