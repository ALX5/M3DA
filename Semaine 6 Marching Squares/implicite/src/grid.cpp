#include "grid.h"
#include <GL/glew.h>
#include <iostream>

Grid::Grid() {}

void Grid::drawGrid() {
    glLineWidth(1);
    glColor3f(0,0,0);
    for(double i=-1.0; i<1.0; i+=2.0/_x) {
        glBegin(GL_LINE_STRIP);
        for(double j=1.0; j>-1.0; j-=2.0/_y) glVertex2f(i,j);
        glEnd();
    }
    for(double j=-1.0; j<1.0; j+=2.0/_x) {
        glBegin(GL_LINE_STRIP);
        for(double i=1.0; i>-1.0; i-=2.0/_y) glVertex2f(i,j);
        glEnd();
    }
}

void Grid::drawTops() {
    glPointSize(5);
    for(double i=-1.0; i<1.0; i+=2.0/_x) {
        glBegin(GL_POINTS);
        for(double j=1.0; j>-1.0; j-=2.0/_y) {
            if(_implicit.eval(i,j)<0) glColor3f(0,0,1);
            else glColor3f(1,0,0);
            glVertex2f(i,j);
        }
        glEnd();
    }
    for(double j=-1.0; j<1.0; j+=2.0/_x) {
        glBegin(GL_POINTS);
        for(double i=1.0; i>-1.0; i-=2.0/_y) {
            if(_implicit.eval(i,j)<0) glColor3f(0,0,1);
            else glColor3f(1,0,0);
            glVertex2f(i,j);
        }
        glEnd();
    }
}

void Grid::savePoints() {
    int iterator = 0;
    do {
        for(double i=-1.0; i<1.0; i+=2.0/_x)
            for(double j=1.0; j>-1.0; j-=2.0/_y) {
                if(iterator<_P.size()) _P.at(iterator)=Vector2(i,j);
                iterator++;
            }
    } while(iterator<_P.size());
}
