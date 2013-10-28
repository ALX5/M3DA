#ifndef VECTOR2_H
#define VECTOR2_H

/* Implémentation des vecteurs 2D */
class Vector2  {

public:
    double x,y;

    Vector2() {x = y = 0.0;}
    Vector2(const Vector2 & v) {x = v.x; y = v.y;}
    Vector2(double a,double b) {x = a;y = b;}

    double getX() const {return x;}
    double getY() const {return y;}

    // Opérateur de l'espace vectoriel
    Vector2 operator+(const Vector2 & v) const;
    Vector2 operator*(const double & d) const;

};

#endif // VECTOR2_H
