#include "limelib/object2d.hpp"

bool limelib::Object2D::isContacting(Pose2D pose) const
{
    return isContacting(Point2D(pose.x, pose.y));
}
bool limelib::Object2D::isContacting(real_t x, real_t y) const
{
    return isContacting(Point2D(x, y));
}


limelib::Rectangle2D::Rectangle2D(Pose2D pose, real_t width, real_t height)
    : width(width), height(height), pose(pose)
{
    Rectangle2D(pose.x, pose.y, width, height, pose.theta);
}
limelib::Rectangle2D::Rectangle2D(real_t x, real_t y, real_t width, real_t height, real_t theta)
    : width(width), height(height), pose(x, y, theta)
{
    std::vector<Point2D> corners = {
        Point2D(x,y),
        Point2D(cos(theta)*width/2+x, sin(theta)*width/2+y),
        Point2D(cos(theta)*width/2+x-cos(theta)*height/2, sin(theta)*width/2+y), // Adjusted
        Point2D(cos(theta)*width/2+x, sin(theta)*width/2+y)
    };
    edges = {
        LineSegment2D(x - width / 2, y - height / 2, x + width / 2, y - height / 2),
        LineSegment2D(x + width / 2, y - height / 2, x + width / 2, y + height / 2),
        LineSegment2D(x + width / 2, y + height / 2, x - width / 2, y + height / 2),
        LineSegment2D(x - width / 2, y + height / 2, x - width / 2, y - height / 2)
    };
}


