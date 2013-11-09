#include <cmath>
#include <iostream>

#include "implicit.h"

Implicit::Implicit() { }

void Implicit::initializeListBlobs() {
    _listBlobs=std::vector<Blob>(0);
}

double Implicit::eval(double x, double y) {
    return pow(x,2)+pow(y,2)-1;
}

void Implicit::addBlob(double a, double b, Vector2 center, double radius) {
    std::vector<Blob> newList(_listBlobs.size()+1);
    int i=0;
    for(; i<_listBlobs.size(); i++) {
        newList.at(i)=_listBlobs.at(i);
    }
    newList.at(i)=Blob(a,b,center,radius);
    _listBlobs=newList;
}

double Implicit::evalBlobsPoints(double x, double y) {
    double eval = 0;
    for (int i = 0; i < _listBlobs.size(); ++i) {
        double distance = _listBlobs.at(i).center.distance(x, y);
        eval += _listBlobs.at(i).a*std::exp(-(_listBlobs.at(i).b*(distance/_listBlobs.at(i).radius)));
    }
    return 1 - eval;
}
