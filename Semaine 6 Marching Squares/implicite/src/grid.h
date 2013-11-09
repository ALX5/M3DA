#ifndef GRID_H
#define GRID_H

#include <vector>
#include "implicit.h"
#include "vector2.h"
#include "blob.h"

using namespace std;
class Grid
{
public:
    Grid();
    Grid(int x, int y) { _x=x-1.0; _y=y-1.0; this->savePoints(); _implicit=Implicit(); __ambiguous=1; }

    int _x, _y;
    int __ambiguous;

    void drawGrid();
    void drawTops();
    void drawCurvePoints();
    void savePoints();
    void drawCurve();
    void drawBlobsPoints();
    void drawBlobs();
    void computeCurveSegmentWithConfiguration3(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 n); // p for the positive point & n for negative (or inversly when configuration==1)
    void computeCurveSegmentWithConfiguration2(Vector2 p1, Vector2 p2, Vector2 n1, Vector2 n2);
    void computeAmbiguousCase(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3);

    double linearInterpolation(double a, double b, double c);
    double computeInterpolationCoefficient(Vector2 point1, Vector2 point2);



    Implicit _implicit;

    // Liste des points de la grille.
    Vector2 **_grid;
};

#endif // GRID_H
