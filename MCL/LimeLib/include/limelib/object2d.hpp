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
        Object2D() = default;
        Object2D(const Object2D &) = delete; // Prevent copying
        Object2D &operator=(const Object2D &) = delete;
        Object2D(Object2D &&) = default; // Allow moving
        Object2D &operator=(Object2D &&) = default;
        virtual ~Object2D() = default;

        // bool isContacting(Pose2D pose) const;
        // bool isContacting(real_t x, real_t y) const;
        virtual bool isContacting(Point2D point) const = 0;
        virtual const std::vector<LineSegment2D> &getEdges() const = 0;
        // virtual const std::vector<Point2D> &getCorners() const;
    };

    class Rectangle2D : public Object2D
    {
    public:
        Rectangle2D(real_t x, real_t y, real_t width, real_t height, real_t theta = 0);
        Rectangle2D(Pose2D, real_t width, real_t height);
        Rectangle2D(const Rectangle2D &) = delete;
        Rectangle2D &operator=(const Rectangle2D &) = delete;
        Rectangle2D(Rectangle2D &&) = default;
        Rectangle2D &operator=(Rectangle2D &&) = default;
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
        Circle2D(const Circle2D &) = delete;
        Circle2D &operator=(const Circle2D &) = delete;
        Circle2D(Circle2D &&) = default;
        Circle2D &operator=(Circle2D &&) = default;
        bool isContacting(Point2D pose) const override;
        const std::vector<LineSegment2D> &getEdges() const override;
        // const std::vector<Point2D> &getCorners() const override { return corners; }

    private:
        real_t radius;
        mutable std::vector<LineSegment2D> edges;
        mutable std::vector<Point2D> corners;
        Point2D pose;
        void initializeEdges() const;
    };

    class Polygon2D : public Object2D
    {
    public:
        Polygon2D(const std::vector<Point2D> &corners);
        Polygon2D(const Polygon2D &) = delete;
        Polygon2D &operator=(const Polygon2D &) = delete;
        Polygon2D(Polygon2D &&) = default;
        Polygon2D &operator=(Polygon2D &&) = default;
        bool isContacting(Point2D point) const override;
        const std::vector<LineSegment2D> &getEdges() const override;
        // const std::vector<Point2D> &getCorners() const override { return corners; }

    private:
        std::vector<Point2D> corners;
        std::vector<LineSegment2D> edges;
        void constructEdges();
        bool pointInPolygon(const Point2D &point) const;
    };

    bool operator==(const std::shared_ptr<Object2D> &a, const std::shared_ptr<Object2D> &b);
    class ObjectGroup2D final : public Object2D
    {
    public:
        ObjectGroup2D() = default;
        explicit ObjectGroup2D(std::vector<std::shared_ptr<Object2D>> objects);
        ~ObjectGroup2D() override = default;

        ObjectGroup2D(const ObjectGroup2D &) = delete;
        ObjectGroup2D &operator=(const ObjectGroup2D &) = delete;
        ObjectGroup2D(ObjectGroup2D &&) = default;
        ObjectGroup2D &operator=(ObjectGroup2D &&) = default;

        void addObject(std::shared_ptr<Object2D> object);
        void removeObject(const std::shared_ptr<Object2D> &object);
        bool isContacting(Point2D point) const override;
        const std::vector<LineSegment2D> &getEdges() const override;

    private:
        std::vector<std::shared_ptr<Object2D>> objects;
        mutable std::vector<LineSegment2D> edges;
        void constructEdges() const;
    };
}

#endif