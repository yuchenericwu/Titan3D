#include "Math.h"

#include <cassert>
#include <cmath>
#include <vector>

#include "Point.h"

static double next_value(double curr, double end) {
  return end < curr ? curr - Constants::PRECISION
                    : curr + Constants::PRECISION;
}

static double get_distance(Point a, Point b) {
  double delta_x = a.x() - b.x();
  double delta_y = a.y() - b.y();
  double delta_z = a.z() - b.z();
  return std::sqrt(delta_x*delta_x + delta_y*delta_y + delta_z*delta_z);
}

double round(double n, double precision) {
  return std::floor(n/precision + 0.5) * precision;
}

Vector cross_product(const Vector& a, const Vector& b) {
  double x = a.y()*b.z() - a.z()*b.y();
  double y = a.z()*b.x() - a.x()*b.z();
  double z = a.x()*b.y() - a.y()*b.x();
  return Vector(x, y, z);
}

Point get_next_point_on_line(const Point& curr, const Point& start,
                             const Point& end) {
  if (curr == end)
    return end;

  std::vector<Point> nexts;
  if (curr.x() != end.x())
    nexts.push_back(Point(next_value(curr.x(), end.x()), curr.y(), curr.z()));
  if (curr.y() != end.y())
    nexts.push_back(Point(curr.x(), next_value(curr.y(), end.y()), curr.z()));
  if (curr.z() != end.z())
    nexts.push_back(Point(curr.x(), curr.y(), next_value(curr.z(), end.z())));

  std::vector<double> distances;
  for (const Point& point : nexts)
    distances.push_back(get_distance(start, point) + get_distance(point, end));
  auto minIt = std::min_element(distances.begin(), distances.end());
  return nexts[minIt - distances.begin()];
}

Vector get_point_above_surface(const Vector& a, const Vector& b) {
  // Values are multiplied by 100 to reduce rounding error.
  Vector n = cross_product(a * 100, b * 100);
  Point origin;
  return get_next_point_on_line(origin, origin, origin + n);
}
