#ifndef NURBS_H
#define NURBS_H

#include <stdio.h>
#include <vector>

#include <iostream>

class Nurbs
{
public:
    Nurbs();
    void initialize(int m, int p);
    double N(int k, int p, int t);
    void drawN(int k, int p);

    // Tableau représentant le vecteur nodal.
    std::vector<double> _knot;

    // Degré de la courbe.
    int _degre;
};

#endif // NURBS_H
