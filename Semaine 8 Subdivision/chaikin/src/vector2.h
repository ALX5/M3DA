#ifndef VECTOR2_H
#define VECTOR2_H

class Vector2  {

public:
    double x,y;
    bool isFixed;

    Vector2() {x = y = 0.0; isFixed=false; }
    Vector2(const Vector2 & v) {x = v.x; y = v.y; isFixed=v.isFixed; }
    Vector2(double a,double b, bool isFixed) {x = a;y = b; this->isFixed=isFixed; }

    double getX() const {return x;}
    double getY() const {return y;}

    Vector2 operator+(const Vector2 & v) const;
    Vector2 operator*(const double & d) const;

    double distance(double x, double y);

};

#endif // VECTOR2_H
