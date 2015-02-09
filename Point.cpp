#include "Point.h"

#include <ostream>
#include <utility>

#include "Math.h"

Point::Point(double x, double y, double z) :
    x_(x), y_(y), z_(z) {
  trim();
}

bool operator==(const Point& lhs, const Point& rhs) {
  return lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_ && lhs.z_ == rhs.z_;
}

bool operator!=(const Point& lhs, const Point& rhs) {
  return !(lhs == rhs);
}

Point& Point::operator+=(Point rhs) {
  x_ += rhs.x_;
  y_ += rhs.y_;
  z_ += rhs.z_;
  return *this;
}

Point& Point::operator-=(Point rhs) {
  x_ -= rhs.x_;
  y_ -= rhs.y_;
  z_ -= rhs.z_;
  return *this;
}

Point& Point::operator*=(double n) {
  x_ *= n;
  y_ *= n;
  z_ *= n;
  trim();
  return *this;
}

Point operator*(Point lhs, double n) {
  return lhs *= n;
}

void Point::trim() {
  x_ = round(x_, Constants::PRECISION);
  y_ = round(y_, Constants::PRECISION);
  z_ = round(z_, Constants::PRECISION);
}

std::ostream& operator<<(std::ostream& out, const Point& point) {
  out << "(" << point.x() << ", " << point.y() << ", " << point.z() << ")";
  return out;
}
