#pragma once

#ifndef LIMELIB_OBJECT2D_HPP
#define LIMELIB_OBJECT2D_HPP

#include "limelib/pose2d.hpp"
#include "limelib/point2d.hpp"
#include "limelib/lineSegment2d.hpp"
#include "limelib/types.hpp"
#include <fstream>
 
namespace limelib
{
    Point2D rotatePoint(Point2D point, Point2D origin, real_t angle);
    class Object2D
    {
    public:
        virtual ~Object2D() = default;
        bool isContacting(Pose2D pose) const;
        bool isContacting(real_t x, real_t y) const;
        virtual bool isContacting(Point2D point) const;
        virtual const std::vector<LineSegment2D> &getEdges() const;
        // virtual const std::vector<Point2D> &getCorners() const;
    };


    class Rectangle2D : public Object2D
    {
    public:
        Rectangle2D(real_t x, real_t y, real_t width, real_t height, real_t theta = 0);
        Rectangle2D(Pose2D, real_t width, real_t height);
        bool isContacting(Point2D point) const override;
        const std::vector<LineSegment2D> &getEdges() const override;
        // const std::vector<Point2D> &getCorners() const override { return corners; }

    private:
        real_t width;
        real_t height;
        std::vector<LineSegment2D> edges;
        std::vector<Point2D> corners;
        Pose2D pose;
    };

    class Circle2D : public Object2D
    {
    public:
        Circle2D(real_t x, real_t y, real_t radius)
            : pose(x, y), radius(radius) {};
        bool isContacting(Point2D pose) const override;
        const std::vector<LineSegment2D> &getEdges() const override;
        // const std::vector<Point2D> &getCorners() const override { return corners; }

    private:
        real_t radius;
        std::vector<LineSegment2D> edges;
        std::vector<Point2D> corners;
        Point2D pose;
    };

    class Polygon2D : public Object2D
    {
    public:
        Polygon2D(const std::vector<Point2D> &corners);
        bool isContacting(Point2D point) const override;
        const std::vector<LineSegment2D> &getEdges() const override;
        // const std::vector<Point2D> &getCorners() const override { return corners; }

    private:
        std::vector<Point2D> corners;
        std::vector<LineSegment2D> edges;
        void constructEdges();
        bool pointInPolygon(const Point2D &point) const;
    };

    bool operator==(const limelib::Object2D &a, const limelib::Object2D &b);
    class ObjectGroup2D : public Object2D
    {
    public:
        ObjectGroup2D(const std::vector<std::shared_ptr<Object2D>> &objects = {});
        void addObject(std::shared_ptr<Object2D> object);
        void removeObject(std::shared_ptr<Object2D> object);
        bool isContacting(Point2D point) const override;
        const std::vector<LineSegment2D> &getEdges() const override;
        const std::vector<std::shared_ptr<Object2D>> &getObjects() const { return objects; }
        // const std::vector<Point2D> &getCorners() const override { return corners; }
        size_t size() const { return objects.size(); }

    private:
        std::vector<std::shared_ptr<Object2D>> objects;
        std::vector<LineSegment2D> edges;
        std::vector<Point2D> corners;
        void constructEdges();
    };

}

#endif