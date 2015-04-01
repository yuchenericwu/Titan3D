#include "StdAfx.h"
#include "Vector.h"

#include <ostream>
#include <utility>

#include "Math.h"

VectorBase& VectorBase::operator+=(VectorBase rhs) {
  x_ += rhs.x_;
  y_ += rhs.y_;
  z_ += rhs.z_;
  return *this;
}

VectorBase& VectorBase::operator-=(VectorBase rhs) {
  x_ -= rhs.x_;
  y_ -= rhs.y_;
  z_ -= rhs.z_;
  return *this;
}

void Vector::trim() {
  x_ = round(x_, Constants::PRECISION);
  y_ = round(y_, Constants::PRECISION);
  z_ = round(z_, Constants::PRECISION);
}

bool operator==(const VectorBase& lhs, const VectorBase& rhs) {
  return lhs.x() == rhs.x() && lhs.y() == rhs.y() && lhs.z() == rhs.z();
}

bool operator!=(const VectorBase& lhs, const VectorBase& rhs) {
  return !(lhs == rhs);
}

bool operator<(const VectorBase& lhs, const VectorBase& rhs) {
  if (lhs.z() < rhs.z()) return true;
  if (lhs.z() > rhs.z()) return false;
  if (lhs.y() < rhs.y()) return true;
  if (lhs.y() > rhs.y()) return false;
  if (lhs.x() < rhs.x()) return true;
  return false;
}

VectorBase operator+(VectorBase lhs, const VectorBase& rhs) {
  return lhs += rhs;
}

VectorBase operator-(VectorBase lhs, const VectorBase& rhs) {
  return lhs -= rhs;
}

VectorBase operator*(VectorBase lhs, double n) {
  return VectorBase(lhs.x() * n, lhs.y() * n, lhs.z() *n);
}

std::ostream& operator<<(std::ostream& out, const VectorBase& point) {
  out << "(" << point.x() << ", " << point.y() << ", " << point.z() << ")";
  return out;
}
