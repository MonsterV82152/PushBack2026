#pragma once

#ifndef MCLDISTANCE_HPP
#define MCLDISTANCE_HPP

#include "limelib/types.hpp"
#include "limelib/pose2d.hpp"
#include "pros/distance.hpp"

namespace limelib
{
    class MCLDistance
    {
    public:
        // Allow nullptr construction with default values
        MCLDistance(pros::Distance& sensor, Pose2D pose);

        Pose2D getPose() const;
        real_t getDistance() const;
        

    private:
        pros::Distance& sensor;
        Pose2D pose;
    };
}

#endif