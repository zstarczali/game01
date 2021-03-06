#include "point.hpp"

#include <cmath>
#include <limits>

using Shape::Point;

Point::Point() : x(0.0), y(0.0)
{
}

Point::~Point()
{
}

void Point::set_x(double x)
{
    this->x = x;
}

void Point::set_y(double y)
{
    this->y = y;
}

double Point::get_x()
{
    return x;
}

double Point::get_y()
{
    return y;
}

Point::Point(double x, double y)
{
    this->x = x;
    this->y = y;
}

Point Point::operator=(const Point &other)
{
    if (this != &other)
    {
        this->x = other.x;
        this->y = other.y;
    }
}

bool Point::isApproximatelyEqual(double x, double y) const
{
    return fabs(x - y) <= numeric_limits<float>::epsilon();
}

bool Point::operator==(const Point &other) const
{
    if (this->isApproximatelyEqual(this->x, other.x) && this->isApproximatelyEqual(this->y, other.y))
        return true;
    return false;
}

// comparison is done first on y coordinate and then on x coordinate
bool Point::operator<(const Point &b) const
{
    if (y != b.y)
        return y < b.y;
    return x < b.x;
}
