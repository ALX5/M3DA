#ifndef NURBS_H
#define NURBS_H

#include <stdio.h>
#include <vector>
#include "vector2.h"

#include <iostream>

using namespace std;

class Nurbs
{
public:
    Nurbs();
    void initialize(int m, int p);
    double N(int k, int p, double t);
    void drawN(int k, int p);
    void drawAllN(int p);
    void drawControlPolygone(vector<Vector2> controlPoints);
    Vector2 evalBSpline(double t, int p);
    void drawBSpline(vector<Vector2> P, int p);
    void runThroughCurves(vector<Vector2> P, int p);
    void initializeNotUniformNodalVector(int m, int p);


    // Tableau représentant le vecteur nodal.
    vector<double> _knot;

    // Question 7 : Tableau de points de contrôle.
    vector<Vector2> _P;

    // Degré de la courbe.
    int _degre;
};

#endif // NURBS_H
