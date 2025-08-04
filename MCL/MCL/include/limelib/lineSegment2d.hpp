#ifndef LIMELIB_LINESEGMENT2D_HPP
#define LIMELIB_LINESEGMENT2D_HPP

#include "limelib/pose2d.hpp"
#include "limelib/point2d.hpp"
#include "limelib/types.hpp"

namespace limelib
{
    struct LineSegment2D
    {
        Point2D start, end;
        LineSegment2D(Point2D start, Point2D end) : start(start), end(end) {}
        LineSegment2D(real_t startX, real_t startY, real_t endX, real_t endY)
            : start(startX, startY), end(endX, endY) {}
    };

}

#endif