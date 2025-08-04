#pragma once

#ifndef LIMELIB_FIELD2D_HPP
#define LIMELIB_FIELD2D_HPP
#include "limelib/object2d.hpp"

namespace limelib
{
    class Field2D
    {
    public:
        Field2D(real_t width, real_t height, std::vector<Object2D> objects);
        void addObject(Object2D object);
        real_t getWidth() const;
        real_t getHeight() const;
        std::vector<Object2D> getObjects() const;
        std::vector<LineSegment2D> getEdges() const;

    private:
        real_t width;
        real_t height;
        std::vector<Object2D> objects;
        std::vector<LineSegment2D> edges; 
        
    };

}
#endif