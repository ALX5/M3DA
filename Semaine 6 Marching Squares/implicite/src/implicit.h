#ifndef IMPLICIT_H
#define IMPLICIT_H

#include <vector>
#include "vector2.h"
#include "blob.h"

class Implicit
{
public:
    Implicit();
    std::vector<Blob> _listBlobs;

    void initializeListBlobs();
    double eval(double x, double y);
    double evalBlobsPoints(double x, double y);
    void addBlob(double a, double b, Vector2 center, double radius);
};

#endif // IMPLICIT_H
