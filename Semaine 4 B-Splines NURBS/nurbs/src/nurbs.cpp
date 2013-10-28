#include "nurbs.h"

#include <GL/glew.h>

Nurbs::Nurbs() {}

// Question 1
void Nurbs::initialize(int m, int p) {
    int nNodal = m + p + 1;
    std::vector<double> knot(nNodal);
    _knot=knot;
    _degre=p;
    for(int i=0; i<nNodal; i++)
       _knot.at(i)=i+1;
}

// Question 2
double Nurbs::N(int k, int p, double t) {
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

// Question 3
void Nurbs::drawN(int k, int p) {
    glLineWidth(3);
    glColor3f(0,1,0);
    glBegin(GL_LINE_STRIP);
    for(double t=_knot.at(0); t<_knot.at(_knot.size()-1);t+=0.01) {
        glVertex2f(t/_knot.size()-1,N(k,p,t));
    }
    glEnd();
    glPointSize(10);
    glColor3f(0,0,1);
    glBegin(GL_POINTS);
    for(auto t : _knot)
        glVertex2f(t/_knot.size()-1,0);
    glEnd();
}

// Question 4
void Nurbs::drawAllN(int p) {
    for(int k=0; k<_knot.size()-p-1; k++)
        drawN(k,p);
}

// Question 7
void Nurbs::drawControlPolygone(std::vector<Vector2> P)  {
    glLineWidth(1);
    glColor3f(0,0,0);
    glBegin(GL_LINE_STRIP);
    _P=P;
    for (int i=0; i<_P.size(); i++) {
        glVertex2f(_P.at(i).getX()/_knot.size(),_P.at(i).getY());
    }
    glEnd();
    glPointSize(3);
    glColor3f(0,0,0);
    glBegin(GL_POINTS);
    for (int i=0; i<_P.size(); i++) {
        glVertex2f(_P.at(i).getX()/_knot.size(),_P.at(i).getY());
    }
    glEnd();
}

// Question 8
Vector2 Nurbs::evalBSpline(double t, int p) {
    Vector2 Pt=Vector2(0.0,0.0);
    for(int k=0; k<_P.size()-1; k++) {
        Pt=Pt+(_P.at(k)*N(k,p,t));
    }
    return Pt;
}

// Question 9
void Nurbs::drawBSpline(vector<Vector2> P, int p) {
    glLineWidth(3);
    glColor3f(0,1,0);
    glBegin(GL_LINE_STRIP);
    for(double t=_knot.at(p); t<_knot.at(_P.size()); t+=0.01) {
        glVertex2f(evalBSpline(t,p).getX()/_knot.size(),evalBSpline(t,p).getY());
    }
    glEnd();
}

// Question 10
void Nurbs::runThroughCurves(int p, double t) {
    glPointSize(5);
    glColor3f(1,0,0);
    glBegin(GL_POINTS);
    glVertex2f(evalBSpline(t,p).getX()/_knot.size(),evalBSpline(t,p).getY());
    glVertex2f(t/_knot.size()-1,0);
    glEnd();
}

// Question 11
void Nurbs::initializeNotUniformNodalVector(int m, int p) {
    int nNodal = m + p + 1;
    std::vector<double> knot(nNodal);
    _knot=knot;
    _degre=p;
    for(int i=0; i<nNodal; i++)
       _knot.at(i)=i-nNodal/2;
}
