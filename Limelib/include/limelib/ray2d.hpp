#pragma once

#ifndef LIMELIB_RAY2D_HPP
#define LIMELIB_RAY2D_HPP

#include "limelib/point2d.hpp"
namespace limelib
{
    struct Ray2D
    {
        Point2D start;
        real_t radians;
        Ray2D(Point2D start = Point2D(), real_t radians = 0) : start(start), radians(radians) {}
    };

}

#endif