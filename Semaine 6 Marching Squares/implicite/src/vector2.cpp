#include "vector2.h"
#include <stdio.h>
#include <math.h>
#include <cmath>

Vector2 Vector2::operator+(const Vector2 & v) const
{
    return Vector2(x+v.x,y+v.y);
}

Vector2 Vector2::operator*(const double & d) const
{
    return Vector2(d * x , d * y);
}

double Vector2::distance(double x, double y) {
    return std::sqrt(std::pow(x-this->x, 2)+std::pow(y-this->y, 2));
}
