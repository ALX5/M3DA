#include "nurbs.h"

#include <GL/glew.h>

Nurbs::Nurbs() {}

void Nurbs::initialize(int m, int p) {
    int nNodal = m + p + 1;
    std::vector<double> knot(nNodal);
    _knot=knot;
    _degre=p;
    for(int i=0; i<nNodal; i++)
       _knot.at(i)=i+1;
}

double Nurbs::N(int k, int p, int t) {
    if(p==0)
        if(t>=_knot.at(k) && t<_knot.at(k+1))
            return 1;
        else
            return 0;
    else
        return ((t-_knot.at(k))/(_knot.at(k+p)-_knot.at(k)))
            *this->N(k,p-1,t)
            +((_knot.at(p+k+1)-t)/(_knot.at(k+p+1)-_knot.at(k+1)))
            *this->N(k+1,p-1,t);
}

void Nurbs::drawN(int k, int p) {
    glLineWidth(3);
    glColor3f(0,1,0);
    glBegin(GL_LINE_STRIP);
    for(auto t : _knot)
        glVertex2f(t/10-0.5,N(k,p,t)-0.5);
    glEnd();
    glPointSize(10);
    glColor3f(0,0,1);
    glBegin(GL_POINTS);
    for(auto t : _knot)
        glVertex2f(t/10-0.5,N(k,p,t)-0.5);
    glEnd();
}
