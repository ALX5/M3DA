#ifndef CHAIKIN_H
#define CHAIKIN_H

#include "glsupport.h"
#include "vector2.h"
#include <vector>

class Chaikin
{
public:
    Chaikin() {
        _curvePoints=std::vector<Vector2>(0);
        _subdividedCurvePoints=std::vector<Vector2>(0);
    }

    // Points of the curve.
    std::vector<Vector2> _curvePoints;
    // Points of the sudbidivded curve.
    std::vector<Vector2> _subdividedCurvePoints;

    void initializeCurves();
    void addPoint(double x, double y, bool isFixed);
    void drawCurves();
    void drawClosedCurves();
    void approximate();
    void interpolate();
};



#endif // CHAIKIN_H
