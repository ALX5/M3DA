#ifndef VECTOR2_H
#define VECTOR2_H

class Vector2  {

public:
    double x,y;

    Vector2() {x = y = 0.0;}
    Vector2(const Vector2 & v) {x = v.x; y = v.y;}
    Vector2(double a,double b) {x = a;y = b;}

    double getX() const {return x;}
    double getY() const {return y;}

    Vector2 operator+(const Vector2 & v) const;
    Vector2 operator*(const double & d) const;

    double distance(double x, double y);

};

#endif // VECTOR2_H
