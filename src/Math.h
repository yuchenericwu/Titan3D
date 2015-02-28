#ifndef MATH_H
#define MATH_H

#include "Vector.h"

double round(double n, double precision);
Vector get_next_point_on_line(const Vector& curr, const Vector& start,
                             const Vector& end, bool simple = false);

/* Currently unused.
Vector cross_product(const Vector& a, const Vector& b);
Vector get_point_above_surface(const Vector& a, const Vector& b);
*/

#endif // MATH_H
