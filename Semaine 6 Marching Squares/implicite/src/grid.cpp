#include "grid.h"
#include <glsupport.h>
#include <iostream>
#include <math.h>

Grid::Grid() {}

void Grid::drawGrid() {
    glLineWidth(1);
    glColor3f(0,0,0);
    for(double i=-1.0; i<=1.1; i+=2.0/_x) {
        glBegin(GL_LINE_STRIP);
        for(double j=1.0; j>=-1.1; j-=2.0/_y) glVertex2f((float)i,(float)j);
        glEnd();
    }
    for(double j=-1.0; j<=1.1; j+=2.0/_x) {
        glBegin(GL_LINE_STRIP);
        for(double i=1.0; i>=-1.1; i-=2.0/_y) glVertex2f(i,j);
        glEnd();
    }
}

void Grid::drawTops() {
    glPointSize(5);
    for(double i=-1.0; i<=1.1; i+=2.0/_x) {
        glBegin(GL_POINTS);
        for(double j=1.0; j>=-1.1; j-=2.0/_y) {
            if(_implicit.eval(i,j)<0) glColor3f(0,0,1);
            else glColor3f(1,0,0);
            glVertex2f(i,j);
        }
        glEnd();
    }
    for(double j=-1.0; j<=1.1; j+=2.0/_x) {
        glBegin(GL_POINTS);
        for(double i=1.0; i>=-1.1; i-=2.0/_y) {
            if(_implicit.eval(i,j)<0) glColor3f(0,0,1);
            else glColor3f(1,0,0);
            glVertex2f(i,j);
        }
        glEnd();
    }
}

void Grid::savePoints() {
    _grid = new Vector2* [_x+1];
    for (int i = 0; i < _x+1; ++i) {
        _grid[i] = new Vector2[_y+1];
    }
    int k=0;
    int l=0;
    for(double i=-1.0; i<=1.1; i+=2.0/_x) {
    l=0;
        for(double j=1.0; j>=-1.1; j-=2.0/_y) {
            _grid[k][l]=Vector2(i, j);
            l++;
        }
        k++;
    }
}

double Grid::linearInterpolation(double a, double b, double c) {
    return a + (b-a) * c;
}

double Grid::computeInterpolationCoefficient(Vector2 point1, Vector2 point2) {
    return std::abs(_implicit.eval(point1.x,point1.y)) / (std::abs(_implicit.eval(point1.x,point1.y)) + std::abs(_implicit.eval(point2.x,point2.y)));
}

void Grid::drawCurvePoints() {
    glColor3f(0,1,0);
    glPointSize(10);
    glBegin(GL_POINTS);
    for(int i=0; i<=_x; i++) {
        for(int j=1; j<=_y; j++) {
            double x = _grid[i][j].x;
            Vector2 previousPoint = _grid[i][j-1];
            Vector2 point = _grid[i][j];
            bool signPreviousPoint = _implicit.eval(previousPoint.x,previousPoint.y) <0;
            bool signPoint = _implicit.eval(point.x,point.y) <0;
            if(signPreviousPoint!=signPoint) {
                double c = computeInterpolationCoefficient(previousPoint, point);
                double y = linearInterpolation(previousPoint.y,point.y,c);
                glVertex2f(x,y);
            }
        }
    }
    for(int j=0; j<=_y; j++) {
        for(int i=1; i<=_x; i++) {
            double y = _grid[i][j].y;
            Vector2 previousPoint = _grid[i-1][j];
            Vector2 point = _grid[i][j];
            bool signPreviousPoint = _implicit.eval(previousPoint.x,previousPoint.y) <0;
            bool signPoint = _implicit.eval(point.x,point.y) <0;
            if(signPreviousPoint!=signPoint) {
                double c = computeInterpolationCoefficient(previousPoint, point);
                double x = linearInterpolation(previousPoint.x,point.x,c);
                glVertex2f(x,y);
            }
        }
    }
    glEnd();
}

void Grid::drawCurve() {
    for(int i=0; i<_x; i++) {
        for(int j=0; j<_y; j++) {
            Vector2 top0 = _grid[i][j];
            Vector2 top1 = _grid[i+1][j];
            Vector2 top2 = _grid[i+1][j+1];
            Vector2 top3 = _grid[i][j+1];
            int signTop0 = _implicit.eval(top0.x,top0.y)>0;
            int signTop1 = _implicit.eval(top1.x,top1.y)>0;
            int signTop2 = _implicit.eval(top2.x,top2.y)>0;
            int signTop3 = _implicit.eval(top3.x,top3.y)>0;
            int configuration = signTop0+signTop1+signTop2+signTop3;
            if(configuration==3) {
                if(signTop0==0) {
                    computeCurveSegmentWithConfiguration3(top1,top2,top3,top0);
                } else if(signTop1==0) {
                    computeCurveSegmentWithConfiguration3(top0,top2,top3,top1);
                } else if(signTop2==0) {
                     computeCurveSegmentWithConfiguration3(top0,top1,top3,top2);
                } else {
                     computeCurveSegmentWithConfiguration3(top0,top1,top2,top3);
                }
            }
            else if(configuration==2) {
                if(signTop1==0 && signTop2==0) {
                    computeCurveSegmentWithConfiguration2(top0,top3,top1,top2);
                } else if(signTop0==0 && signTop3==0) {
                    computeCurveSegmentWithConfiguration2(top1,top2,top0,top3);
                } else if(signTop0==0 && signTop1==0) {
                    computeCurveSegmentWithConfiguration2(top2,top3,top0,top1);
                } else if(signTop2==0 && signTop3==0) {
                    computeCurveSegmentWithConfiguration2(top0,top1,top2,top3);
                } else if((signTop0==0 && signTop2==0) || (signTop1==0 && signTop3==0)) {
                    computeAmbiguousCase(top0,top1,top2,top3);
                }
            }
            else if(configuration==1) {
                if(signTop0==1) {
                    computeCurveSegmentWithConfiguration3(top1,top2,top3,top0);
                } else if(signTop1==1) {
                    computeCurveSegmentWithConfiguration3(top0,top2,top3,top1);
                } else if(signTop2==1) {
                     computeCurveSegmentWithConfiguration3(top0,top1,top3,top2);
                } else {
                     computeCurveSegmentWithConfiguration3(top0,top1,top2,top3);
                }
            }
        }
    }
}

void Grid::computeCurveSegmentWithConfiguration3(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 n) {
    double x1=0.0,y1=0.0,x2=0.0,y2=0.0;
    if(p1.x==n.x) {
        x1=p1.x;
        double c = computeInterpolationCoefficient(p1, n);
        y1 = linearInterpolation(p1.y,n.y,c);
    } else if(p2.x==n.x) {
        x1=p2.x;
        double c = computeInterpolationCoefficient(p2, n);
        y1 = linearInterpolation(p2.y,n.y,c);
    } if(p3.x==n.x) {
        x1=p3.x;
        double c = computeInterpolationCoefficient(p3, n);
        y1 = linearInterpolation(p3.y,n.y,c);
    }
    if(p1.y==n.y) {
        y2=p1.y;
        double c = computeInterpolationCoefficient(p1, n);
        x2 = linearInterpolation(p1.x,n.x,c);
    } else if(p2.y==n.y) {
        y2=p2.y;
        double c = computeInterpolationCoefficient(p2, n);
        x2 = linearInterpolation(p2.x,n.x,c);
    } if(p3.y==n.y) {
        y2=p3.y;
        double c = computeInterpolationCoefficient(p3, n);
        x2 = linearInterpolation(p3.x,n.x,c);
    }
    glLineWidth(2);
    glColor3f(0,1,0);
    glBegin(GL_LINE_STRIP);
    glVertex2f(x1,y1);
    glVertex2f(x2,y2);
    glEnd();
}

void Grid::computeCurveSegmentWithConfiguration2(Vector2 p1, Vector2 p2, Vector2 n1, Vector2 n2) {
    double x1=0.0,y1=0.0,x2=0.0,y2=0.0;
    if(p1.x==n1.x) {
        x1=p1.x;
        double c = computeInterpolationCoefficient(p1, n1);
        y1 = linearInterpolation(p1.y,n1.y,c);
    } else if(p1.x==n2.x) {
        x1=p1.x;
        double c = computeInterpolationCoefficient(p1, n2);
        y1 = linearInterpolation(p1.y,n2.y,c);
    } else if(p1.y==n1.y) {
        y1=p1.y;
        double c = computeInterpolationCoefficient(p1, n1);
        x1 = linearInterpolation(p1.x,n1.x,c);
    } else if(p1.y==n2.y) {
        y1=p1.y;
        double c = computeInterpolationCoefficient(p1, n2);
        x1 = linearInterpolation(p1.x,n2.x,c);
    }
    if(p2.x==n1.x) {
        x2=p2.x;
        double c = computeInterpolationCoefficient(p2, n1);
        y2 = linearInterpolation(p2.y,n1.y,c);
    } else if(p2.x==n2.x) {
        x2=p2.x;
        double c = computeInterpolationCoefficient(p2, n2);
        y2 = linearInterpolation(p2.y,n2.y,c);
    } else if(p2.y==n1.y) {
        y2=p2.y;
        double c = computeInterpolationCoefficient(p2, n1);
        x2 = linearInterpolation(p2.x,n1.x,c);
    } else if(p2.y==n2.y) {
        y2=p2.y;
        double c = computeInterpolationCoefficient(p2, n2);
        x2 = linearInterpolation(p2.x,n2.x,c);
    }
    glLineWidth(2);
    glColor3f(0,1,0);
    glBegin(GL_LINE_STRIP);
    glVertex2f(x1,y1);
    glVertex2f(x2,y2);
    glEnd();
}

void Grid::drawBlobsPoints() {
    glColor3f(0,1,0);
    glPointSize(10);
    glBegin(GL_POINTS);
    for(int i=0; i<=_x; i++) {
        for(int j=1; j<=_y; j++) {
            double x = _grid[i][j].x;
            Vector2 previousPoint = _grid[i][j-1];
            Vector2 point = _grid[i][j];
            bool signPreviousPoint = _implicit.evalBlobsPoints(previousPoint.x,previousPoint.y) <0;
            bool signPoint = _implicit.evalBlobsPoints(point.x,point.y) <0;
            if(signPreviousPoint!=signPoint) {
                double c = computeInterpolationCoefficient(previousPoint, point);
                double y = linearInterpolation(previousPoint.y,point.y,c);
                glVertex2f(x,y);
            }
        }
    }
    for(int j=0; j<=_y; j++) {
        for(int i=1; i<=_x; i++) {
            double y = _grid[i][j].y;
            Vector2 previousPoint = _grid[i-1][j];
            Vector2 point = _grid[i][j];
            bool signPreviousPoint = _implicit.evalBlobsPoints(previousPoint.x,previousPoint.y) <0;
            bool signPoint = _implicit.evalBlobsPoints(point.x,point.y) <0;
            if(signPreviousPoint!=signPoint) {
                double c = computeInterpolationCoefficient(previousPoint, point);
                double x = linearInterpolation(previousPoint.x,point.x,c);
                glVertex2f(x,y);
            }
        }
    }
    glEnd();
}

void Grid::drawBlobs() {
    for(int i=0; i<_x; i++) {
        for(int j=0; j<_y; j++) {
            Vector2 top0 = _grid[i][j];
            Vector2 top1 = _grid[i+1][j];
            Vector2 top2 = _grid[i+1][j+1];
            Vector2 top3 = _grid[i][j+1];
            int signTop0 = _implicit.evalBlobsPoints(top0.x,top0.y)>0;
            int signTop1 = _implicit.evalBlobsPoints(top1.x,top1.y)>0;
            int signTop2 = _implicit.evalBlobsPoints(top2.x,top2.y)>0;
            int signTop3 = _implicit.evalBlobsPoints(top3.x,top3.y)>0;
            int configuration = signTop0+signTop1+signTop2+signTop3;
            if(configuration==3) {
                if(signTop0==0) {
                    computeCurveSegmentWithConfiguration3(top1,top2,top3,top0);
                } else if(signTop1==0) {
                    computeCurveSegmentWithConfiguration3(top0,top2,top3,top1);
                } else if(signTop2==0) {
                     computeCurveSegmentWithConfiguration3(top0,top1,top3,top2);
                } else {
                     computeCurveSegmentWithConfiguration3(top0,top1,top2,top3);
                }
            }
            else if(configuration==2) {
                if(signTop1==0 && signTop2==0) {
                    computeCurveSegmentWithConfiguration2(top0,top3,top1,top2);
                } else if(signTop0==0 && signTop3==0) {
                    computeCurveSegmentWithConfiguration2(top1,top2,top0,top3);
                } else if(signTop0==0 && signTop1==0) {
                    computeCurveSegmentWithConfiguration2(top2,top3,top0,top1);
                } else if(signTop2==0 && signTop3==0) {
                    computeCurveSegmentWithConfiguration2(top0,top1,top2,top3);
                } else if((signTop0==0 && signTop2==0) || (signTop1==0 && signTop3==0)) {
                    computeAmbiguousCase(top0,top1,top2,top3);
                }
            }
            else if(configuration==1) {
                if(signTop0==1) {
                    computeCurveSegmentWithConfiguration3(top1,top2,top3,top0);
                } else if(signTop1==1) {
                    computeCurveSegmentWithConfiguration3(top0,top2,top3,top1);
                } else if(signTop2==1) {
                     computeCurveSegmentWithConfiguration3(top0,top1,top3,top2);
                } else {
                     computeCurveSegmentWithConfiguration3(top0,top1,top2,top3);
                }
            }
        }
    }
}

void Grid::computeAmbiguousCase(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3) {
    double x1=0.0, x2=0.0, x3=0.0, x4=0.0;
    double y1=0.0, y2=0.0, y3=0.0, y4=0.0;
    double c=0.0;
    c=computeInterpolationCoefficient(p0, p1);
    x1 = linearInterpolation(p0.x,p1.x,c);
    y1=p0.y;
    c=computeInterpolationCoefficient(p0, p3);
    x2 = p0.x;
    y2 = linearInterpolation(p0.y,p3.y,c);
    c=computeInterpolationCoefficient(p1, p2);
    x3 = p1.x;
    y3 = linearInterpolation(p1.y,p2.y,c);
    c=computeInterpolationCoefficient(p3, p2);
    x4 = linearInterpolation(p3.x,p2.x,c);
    y4=p3.y;
    glLineWidth(2);
    glColor3f(0,1,0);
    glBegin(GL_LINE_STRIP);
    if(__ambiguous==1) {
        glVertex2f(x1,y1);
        glVertex2f(x2,y2);
    } else {
        glVertex2f(x1,y1);
        glVertex2f(x3,y3);
    }
    glEnd();
    glLineWidth(2);
    glColor3f(0,1,0);
    glBegin(GL_LINE_STRIP);
    if(__ambiguous==1) {
        glVertex2f(x3,y3);
        glVertex2f(x4,y4);
    } else {
        glVertex2f(x2,y2);
        glVertex2f(x4,y4);
    }
    glEnd();
}
