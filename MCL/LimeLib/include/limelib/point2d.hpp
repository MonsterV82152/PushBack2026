#pragma once

#ifndef LIMELIB_POINT2D_HPP
#define LIMELIB_POINT2D_HPP

#include "limelib/types.hpp"

namespace limelib
{
    struct Point2D
    {
        real_t x;
        real_t y;
        Point2D(real_t x = 0, real_t y = 0) : x(x), y(y) {}
    };
}

#endif // LIMELIB_POSE2D_HPP