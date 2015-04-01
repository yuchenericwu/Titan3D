#pragma once

#include <ostream>
#include <utility>

#include "Constants.h"

class VectorBase {
 public:
  VectorBase() : x_(0), y_(0), z_(0) {}
  VectorBase(double x, double y, double z) : x_(x), y_(y), z_(z) {}
  VectorBase(const VectorBase& other) = default;
  VectorBase& operator=(const VectorBase& other) = default;

  double x() const { return x_; }
  double y() const { return y_; }
  double z() const { return z_; }

  VectorBase& operator+=(VectorBase rhs);
  VectorBase& operator-=(VectorBase rhs);

 protected:
  double x_, y_, z_;
};

class Vector : public VectorBase {
 public:
  Vector() : VectorBase() {}
  Vector(double x, double y, double z) : VectorBase(x, y, z) { trim(); }
  Vector(const VectorBase& other) : VectorBase(other) { trim(); }
 private:
  void trim();
};

bool operator==(const VectorBase& lhs, const VectorBase& rhs);
bool operator!=(const VectorBase& lhs, const VectorBase& rhs);
bool operator<(const VectorBase& lhs, const VectorBase& rhs);

VectorBase operator+(VectorBase lhs, const VectorBase& rhs);
VectorBase operator-(VectorBase lhs, const VectorBase& rhs);
VectorBase operator*(VectorBase lhs, double n);
std::ostream& operator<<(std::ostream& out, const VectorBase& point);
