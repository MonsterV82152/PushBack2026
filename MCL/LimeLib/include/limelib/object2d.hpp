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
        Object2D(real_t x = 0, real_t y = 0) : x(x), y(y) {}
        virtual ~Object2D() = default;
        bool isContacting(Pose2D pose) const;
        bool isContacting(real_t x, real_t y) const;
        virtual bool isContacting(Point2D pose) const;
        virtual bool isContacting(Object2D *other) const;
        virtual std::vector<LineSegment2D> getEdges() const;
    private:
        real_t x;
        real_t y;
    };

    class Rectangle2D : public Object2D
    {
    public:
        Rectangle2D(real_t x, real_t y, real_t width, real_t height, real_t theta = 0) {};
        Rectangle2D(Pose2D, real_t width, real_t height) {};
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