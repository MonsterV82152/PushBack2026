#pragma once

#ifndef LIMELIB_OBJECT2D_HPP
#define LIMELIB_OBJECT2D_HPP

#include "limelib/pose2d.hpp"
#include "limelib/point2d.hpp"
#include "limelib/lineSegment2d.hpp"
#include "limelib/types.hpp"


namespace limelib
{
    class Object2D
    {
    public:
        virtual ~Object2D() = default;
        bool isContacting(Pose2D pose) const;
        bool isContacting(real_t x, real_t y) const;
        virtual bool isContacting(Point2D pose) const = 0;
        virtual bool isContacting(Object2D *other) const = 0;
        virtual std::vector<LineSegment2D> getEdges() const = 0;
    };

    class Rectangle2D : public Object2D
    {
    public:
        Rectangle2D(real_t x, real_t y, real_t width, real_t height, real_t theta = 0)
            : pose(x, y, theta), width(width), height(height) {};
        bool isContacting(Point2D pose) const override;
        bool isContacting(Object2D *other) const override;
        std::vector<LineSegment2D> getEdges() const override;


    private:
        real_t width;
        real_t height;
        std::vector<LineSegment2D> edges;
        Pose2D pose;
    };

    class Circle2D : public Object2D
    {
    public:
        Circle2D(real_t x, real_t y, real_t radius)
            : pose(x, y), radius(radius) {};
        bool isContacting(Object2D *other) const override;
        bool isContacting(Point2D pose) const override;
        std::vector<LineSegment2D> getEdges() const override;


    private:
        real_t radius;
        std::vector<LineSegment2D> edges;
        Point2D pose;
    };

    class Polygon2D : public Object2D
    {
        
    };

    class Line2D : public Object2D
    {
    };

    class ObjectGroup2D : public Object2D
    {
    };

}

#endif