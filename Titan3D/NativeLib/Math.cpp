#include "StdAfx.h"
#include "Math.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <set>
#include <utility>
#include <vector>

#include "Constants.h"
#include "Vector.h"

static double next_value(double curr, double end) {
  return (curr == end) ? curr
                       : (curr < end) ? curr + Constants::PRECISION
                                      : curr - Constants::PRECISION;
}

static double get_distance(Vector a, Vector b) {
  double delta_x = a.x() - b.x();
  double delta_y = a.y() - b.y();
  double delta_z = a.z() - b.z();
  return std::sqrt(delta_x*delta_x + delta_y*delta_y + delta_z*delta_z);
}

double round(double n, double precision) {
  return std::floor(n/precision + 0.5) * precision;
}

Vector get_next_point_on_line(const Vector& curr, const Vector& start,
                             const Vector& end, bool simple) {
  if (curr == end)
    return end;

  double next_x = next_value(curr.x(), end.x());
  double next_y = next_value(curr.y(), end.y());
  double next_z = next_value(curr.z(), end.z());

  std::vector<Vector> nexts({Vector(next_x,   curr.y(), curr.z()),
                             Vector(curr.x(), next_y,   curr.z()),
                             Vector(curr.x(), curr.y(), next_z),
                             Vector(next_x,   next_y,   curr.z()),
                             Vector(next_x,   curr.y(), next_z),
                             Vector(curr.x(), next_y,   next_z),
                             Vector(next_x,   next_y,   next_z)});

  std::vector<std::pair<double, int>> distances;
  distances.reserve(nexts.size());
  size_t index = 0;
  for (const Vector& point : nexts) {
    if (point != curr)
      distances.push_back(std::pair<double, int>(
            get_distance(start, point) + get_distance(point, end), index));
    ++index;
  }
  auto minIt = std::min_element(distances.begin(), distances.end());
  return nexts[distances[minIt - distances.begin()].second];
}

/* Currently unused.
Vector cross_product(const Vector& a, const Vector& b) {
  double x = a.y()*b.z() - a.z()*b.y();
  double y = a.z()*b.x() - a.x()*b.z();
  double z = a.x()*b.y() - a.y()*b.x();
  return Vector(x, y, z);
}

Vector get_point_above_surface(const Vector& a, const Vector& b) {
  // Values are multiplied by 100 to reduce rounding error.
  Vector n = cross_product(a * 100, b * 100);
  Vector origin;
  return get_next_point_on_line(origin, origin, origin + n);
}
*/
