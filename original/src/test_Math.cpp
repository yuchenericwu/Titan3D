#include <iostream>
#include <set>

#include "Math.h"
#include "Point.h"

int main() {
  std::set<Point> points;
  Point a(0, 0, 0);
  Point b(1.5, 1.5, -1.5);
  points.insert(a);
  points.insert(b);
  points.insert(Point(0,0,1));
  points.insert(Point(1,0,0));
  for (auto point : points)
    std::cout << point.x() << " " << point.y() << " " << point.z() << "\n";
  return 0;
}
