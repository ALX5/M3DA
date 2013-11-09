#ifndef BLOB_H
#define BLOB_H

#include "vector2.h"

class Blob
{
public:
    Blob();
    Blob(double a, double b, Vector2 center, double radius) {
        this->a=a;
        this->b=b;
        this->center=center;
        this->radius=radius;
    }
    double a, b;
    Vector2 center;
    double radius;

};

#endif // BLOB_H
