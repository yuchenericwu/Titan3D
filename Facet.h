#ifndef FACET_H
#define FACET_H

#include <algorithm>
#include <ostream>
#include <vector>

#include "Point.h"

class Facet {
 public:
  Facet() : normal(), vertices{} {}
  Facet(Vector n, Point v[3]) : normal(n), vertices{v[0], v[1], v[2]} {}

  bool is_valid() const {
    return normal.is_valid() || !vertices[0].is_origin() ||
      !vertices[1].is_origin() || !vertices[2].is_origin();
  }

  float get_x_min() const {
    return std::min({vertices[0].x, vertices[1].x, vertices[2].x});
  }

  float get_x_max() const {
    return std::max({vertices[0].x, vertices[1].x, vertices[2].x});
  }

  float get_y_min() const {
    return std::min({vertices[0].y, vertices[1].y, vertices[2].y});
  }

  float get_y_max() const {
    return std::max({vertices[0].y, vertices[1].y, vertices[2].y});
  }

  float get_z_min() const {
    return std::min({vertices[0].z, vertices[1].z, vertices[2].z});
  }

  float get_z_max() const {
    return std::max({vertices[0].z, vertices[1].z, vertices[2].z});
  }

  friend std::ostream& operator<<(std::ostream& out, const Facet& facet);
 private:
  Vector normal;
  Point vertices[3];
};

std::ostream& operator<<(std::ostream& out, const Facet& facet) {
  out << "Normal: " << facet.normal.x << ", " << facet.normal.y << ", "
    << facet.normal.z << "\n";
  for (int ii = 0; ii < 3; ++ii)
    out << "Vertex " << ii << ": " << facet.vertices[ii].x << ", "
      << facet.vertices[ii].y << ", " << facet.vertices[ii].z << "\n";
  return out;
}

#endif // FACET_H
