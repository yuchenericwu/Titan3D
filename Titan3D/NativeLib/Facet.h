#pragma once

#include <algorithm>

#include "Math.h"
#include "Vector.h"

class Facet {
 public:
  Facet() : normal_(), vertices_{}, checksum_(0) {}
  Facet(const VectorBase& n, const VectorBase v[3], double scaling);

  const Vector normal() const {
    return normal_;
  }

  const Vector* vertices() const {
    return vertices_;
  }

  bool is_valid() const {
    return checksum_;
  }

  double get_x_min() const {
    return std::min({vertices_[0].x(), vertices_[1].x(), vertices_[2].x()});
  }

  double get_x_max() const {
    return std::max({vertices_[0].x(), vertices_[1].x(), vertices_[2].x()});
  }

  double get_y_min() const {
    return std::min({vertices_[0].y(), vertices_[1].y(), vertices_[2].y()});
  }

  double get_y_max() const {
    return std::max({vertices_[0].y(), vertices_[1].y(), vertices_[2].y()});
  }

  double get_z_min() const {
    return std::min({vertices_[0].z(), vertices_[1].z(), vertices_[2].z()});
  }

  double get_z_max() const {
    return std::max({vertices_[0].z(), vertices_[1].z(), vertices_[2].z()});
  }

  int checksum() const {
    return checksum_;
  }
 private:
  Vector normal_;
  Vector vertices_[3];
  int checksum_;
};

bool operator<(const Facet& lhs, const Facet& rhs);
std::ostream& operator<<(std::ostream& out, const Facet& facet);