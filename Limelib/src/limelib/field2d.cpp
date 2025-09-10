#include "limelib/field2d.hpp"

limelib::Field2D::Field2D(real_t width, real_t height, const std::vector<Object2D> &objects) : width(width), height(height), objects(std::move(objects)) {
    calculateEdges();

}

void limelib::Field2D::addObject(const Object2D &object) {
    objects.push_back(object);
    calculateEdges();
}

void limelib::Field2D::removeObject(const Object2D &object) {
    objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());
    calculateEdges();
}
limelib::real_t limelib::Field2D::getWidth() const {
    return width;
}
limelib::real_t limelib::Field2D::getHeight() const {
    return height;
}
std::vector<limelib::Object2D> limelib::Field2D::getObjects() const {
    return objects;
}
std::vector<limelib::LineSegment2D> limelib::Field2D::getEdges() const {
    return edges;
}

void limelib::Field2D::calculateEdges() {
    edges.clear();
    for (const auto& object : objects) {
        auto objectEdges = object.getEdges();
        edges.insert(edges.end(), objectEdges.begin(), objectEdges.end());
    }
    edges.emplace_back(-width/2, -height/2, width/2, -height/2); // Bottom edge
    edges.emplace_back(width/2, -height/2, width/2, height/2); // Right edge
    edges.emplace_back(width/2, height/2, -width/2, height/2); // Top edge
    edges.emplace_back(-width/2, height/2, -width/2, -height/2); // Left edge
}
