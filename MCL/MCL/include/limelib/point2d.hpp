#ifndef LIMELIB_POINT2D_HPP
#define LIMELIB_POINT2D_HPP
#include "limelib/types.hpp"
namespace limelib
{
    class Point2D
    {

    public:
        Point2D(real_t x, real_t y);
        real_t getX() const;
        real_t getY() const;

    private:
        real_t x;
        real_t y;
    };
}

#endif // LIMELIB_POSE2D_HPP