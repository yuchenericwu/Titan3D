#ifndef FACET_H
#define FACET_H

#include <algorithm>
#include <ostream>
#include <vector>

#include "Constants.h"
#include "Point.h"
#include "Units.h"

class Facet {
 public:
  Facet() : normal_(), vertices_{}, valid_(false) {}
  Facet(const Vector& n, const Point v[3], Units units) :
    normal_(n), vertices_{v[0], v[1], v[2]}, valid_(true) {
    if (units == Units::IN) {
      normal_ *= Constants::IN_TO_MM;;
      vertices_[0] *= Constants::IN_TO_MM;
      vertices_[1] *= Constants::IN_TO_MM;
      vertices_[2] *= Constants::IN_TO_MM;
    }
  }

  const Vector normal() const {
    return normal_;
  }

  const Point* vertices() const {
    return vertices_;
  }

  bool is_valid() const {
    return valid_;
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

  friend std::ostream& operator<<(std::ostream& out, const Facet& facet);
 private:
  Vector normal_;
  Point vertices_[3];
  const bool valid_;
};

std::ostream& operator<<(std::ostream& out, const Facet& facet) {
  out << "normal_: " << facet.normal_.x() << ", " << facet.normal_.y() << ", "
    << facet.normal_.z() << "\n";
  for (int ii = 0; ii < 3; ++ii)
    out << "Vertex " << ii << ": " << facet.vertices_[ii].x() << ", "
      << facet.vertices_[ii].y() << ", " << facet.vertices_[ii].z() << "\n";
  return out;
}

#endif // FACET_H
