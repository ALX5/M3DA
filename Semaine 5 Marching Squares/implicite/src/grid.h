#ifndef GRID_H
#define GRID_H

class Grid
{
public:
    Grid();
    Grid(int x, int y) { _x=x; _y=y; }

    int _x, _y;

    void drawGrid();
};

#endif // GRID_H
