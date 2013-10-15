#include "vector2.h"
#include <stdio.h>
#include <math.h>

// Question 6
Vector2 Vector2::operator+(const Vector2 & v) const
{
    return Vector2(x+v.x,y+v.y);
}

Vector2 Vector2::operator*(const double & d) const
{
    return Vector2(d * x , d * y);
}
