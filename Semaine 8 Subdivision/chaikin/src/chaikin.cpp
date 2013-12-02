#include "chaikin.h"

void Chaikin::initializeCurves() {
    _curvePoints=std::vector<Vector2>(0);
    _subdividedCurvePoints=std::vector<Vector2>(0);
}

void Chaikin::addPoint(double x, double y, bool isFixed) {
    _curvePoints.push_back(Vector2(x,y,isFixed));
}

void Chaikin::drawCurves() {
    glColor3f(1,0,0);
    for(int i=0; i<_curvePoints.size(); i++) {
        if(_curvePoints.at(i).isFixed) glPointSize(7);
        else glPointSize(3);
        glBegin(GL_POINTS);
        glVertex2f(_curvePoints.at(i).x,_curvePoints.at(i).y);
        glEnd();
    }
    glLineWidth(1);
    glBegin(GL_LINE_STRIP);
    for(int i=0; i<_curvePoints.size(); i++)
        glVertex2f(_curvePoints.at(i).x,_curvePoints.at(i).y);
    glEnd();

    glColor3f(0,0,1);
    for(int i=0; i<_subdividedCurvePoints.size(); i++) {
        if(_subdividedCurvePoints.at(i).isFixed) glPointSize(7);
        else glPointSize(3);
        glBegin(GL_POINTS);
        glVertex2f(_subdividedCurvePoints.at(i).x,_subdividedCurvePoints.at(i).y);
        glEnd();
    }
    glLineWidth(3);
    glBegin(GL_LINE_STRIP);
    for(int i=0; i<_subdividedCurvePoints.size(); i++)
        glVertex2f(_subdividedCurvePoints.at(i).x,_subdividedCurvePoints.at(i).y);
    glEnd();
}

void Chaikin::drawClosedCurves() {
    glPointSize(3);
    glBegin(GL_POINTS);

    glColor3f(1,0,0);
    for(int i=0; i<_curvePoints.size(); i++)
        glVertex2f(_curvePoints.at(i).x,_curvePoints.at(i).y);
    glEnd();
    glLineWidth(1);
    glBegin(GL_LINE_STRIP);
    for(int i=0; i<_curvePoints.size(); i++)
        glVertex2f(_curvePoints.at(i).x,_curvePoints.at(i).y);
    if(_curvePoints.size()>0) glVertex2f(_curvePoints.at(0).x,_curvePoints.at(0).y);
    glEnd();

    glColor3f(0,0,1);
    for(int i=0; i<_subdividedCurvePoints.size(); i++)
        glVertex2f(_subdividedCurvePoints.at(i).x,_subdividedCurvePoints.at(i).y);
    glEnd();
    glLineWidth(3);
    glBegin(GL_LINE_STRIP);
    for(int i=0; i<_subdividedCurvePoints.size(); i++)
        glVertex2f(_subdividedCurvePoints.at(i).x,_subdividedCurvePoints.at(i).y);
    if(_subdividedCurvePoints.size()>0) glVertex2f(_subdividedCurvePoints.at(0).x,_subdividedCurvePoints.at(0).y);
    glEnd();
}

void Chaikin::approximate() {
    if(_curvePoints.size()>2) {
        if(_subdividedCurvePoints.size()!=0) _curvePoints=_subdividedCurvePoints;
        std::vector<Vector2> subdividedCurvePoints(0);
        for(int i=0; i<_curvePoints.size()-1; i++) {
            if(_curvePoints.at(i).isFixed) subdividedCurvePoints.push_back(_curvePoints.at(i));
            subdividedCurvePoints.push_back(_curvePoints.at(i)*0.75+_curvePoints.at(i+1)*0.25);
            subdividedCurvePoints.push_back(_curvePoints.at(i)*0.25+_curvePoints.at(i+1)*0.75);
        }
        if(_curvePoints.at(_curvePoints.size()-1).isFixed) subdividedCurvePoints.push_back(_curvePoints.at(_curvePoints.size()-1));
        _subdividedCurvePoints=subdividedCurvePoints;
    }
}

void Chaikin::interpolate() {
    if(_curvePoints.size()>2) {
        if(_subdividedCurvePoints.size()!=0) _curvePoints=_subdividedCurvePoints;
        std::vector<Vector2> subdividedCurvePoints(0);
        int i = 0;
        subdividedCurvePoints.push_back(_curvePoints.at(i));
        subdividedCurvePoints.push_back((_curvePoints.at(i+2)+_curvePoints.at(_curvePoints.size()-1))*(-0.0625)+(_curvePoints.at(i+1)+_curvePoints.at(i))*0.5625);
        i++;
        for(; i<_curvePoints.size()-3; i++) {
            subdividedCurvePoints.push_back(_curvePoints.at(i));
            subdividedCurvePoints.push_back((_curvePoints.at(i+2)+_curvePoints.at(i-1))*(-0.0625)+(_curvePoints.at(i+1)+_curvePoints.at(i))*0.5625);
        }
        subdividedCurvePoints.push_back(_curvePoints.at(i));
        subdividedCurvePoints.push_back((_curvePoints.at(0)+_curvePoints.at(i-1))*(-0.0625)+(_curvePoints.at(i+1)+_curvePoints.at(i))*0.5625);
        i++;
        subdividedCurvePoints.push_back(_curvePoints.at(i));
        subdividedCurvePoints.push_back((_curvePoints.at(1)+_curvePoints.at(i-1))*(-0.0625)+(_curvePoints.at(0)+_curvePoints.at(i))*0.5625);
        _subdividedCurvePoints=subdividedCurvePoints;
    }
}
