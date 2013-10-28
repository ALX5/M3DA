#include <cmath>

#include "implicit.h"

Implicit::Implicit() {}

double Implicit::eval(double x, double y) {
    return pow(x,2)+pow(y,2)-1;
}
