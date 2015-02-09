#ifndef MATH_H
#define MATH_H

#include "Point.h"

double round(double n, double precision);
Vector cross_product(const Vector& a, const Vector& b);
Point get_next_point_on_line(const Point& curr, const Point& start,
                             const Point& end);
Vector get_point_above_surface(const Vector& a, const Vector& b);

#endif // MATH_H
