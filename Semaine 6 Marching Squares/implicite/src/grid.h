#ifndef GRID_H
#define GRID_H

#include <vector>
#include "implicit.h"
#include "vector2.h"

using namespace std;
class Grid
{
public:
    Grid();
    Grid(int x, int y) { _x=x; _y=y; _P=vector<Vector2>(_x*_y); this->savePoints(); }

    int _x, _y;

    void drawGrid();
    void drawTops();
    void savePoints();

    Implicit _implicit;

    // Liste des points.
    vector<Vector2> _P;
};

#endif // GRID_H
