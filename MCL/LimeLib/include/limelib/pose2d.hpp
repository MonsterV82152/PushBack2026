#pragma once

#ifndef LIMELIB_POSE2D_HPP
#define LIMELIB_POSE2D_HPP
#include "limelib/types.hpp"

namespace limelib
{
    class Pose2D
    {

    public:
        Pose2D(real_t x, real_t y, real_t theta);
        real_t getX() const;
        real_t getY() const;
        real_t getTheta() const;

    private:
        real_t x;
        real_t y;
        real_t theta;
    };
}

#endif // LIMELIB_POSE2D_HPP