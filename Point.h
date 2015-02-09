#ifndef POINT_H
#define POINT_H

#include <utility>

#include "Constants.h"

class Point {
 public:
  Point() : x_(0), y_(0), z_(0) {}
  Point(double x, double y, double z);


  double x() const { return x_; }
  double y() const { return y_; }
  double z() const { return z_; }

  Point& operator+=(Point rhs);
  Point& operator-=(Point rhs);
  Point& operator*=(double n);

  friend bool operator==(const Point& lhs, const Point& rhs);
  friend bool operator!=(const Point& lhs, const Point& rhs);
  friend Point operator*(Point lhs, double n);

 private:
  void trim();
  double x_, y_, z_;
};

template<typename T_>
Point operator+(Point lhs, const T_& rhs) {
  return lhs += rhs;
}

template<typename T_>
Point operator-(Point lhs, const T_& rhs) {
  return lhs -= rhs;
}

typedef Point Vector;

#endif // POINT_H
