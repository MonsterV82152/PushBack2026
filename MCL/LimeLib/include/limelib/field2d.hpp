#pragma once

#ifndef LIMELIB_FIELD2D_HPP
#define LIMELIB_FIELD2D_HPP
#include "limelib/object2d.hpp"


namespace limelib
{
    class Field2D
    {
    public:
        Field2D(real_t width, real_t height, const std::vector<std::shared_ptr<Object2D>>& objects);
        void addObject(std::shared_ptr<Object2D> object);
        void removeObject(const std::shared_ptr<Object2D>& object);
        real_t getWidth() const;
        real_t getHeight() const;
        const std::vector<std::shared_ptr<Object2D>>& getObjects() const;
        const std::vector<LineSegment2D>& getEdges() const;

    private:
        real_t width;
        real_t height;
        void calculateEdges();
        std::vector<std::shared_ptr<Object2D>> objects;
        mutable std::vector<LineSegment2D> edges;
    };

}
#endif