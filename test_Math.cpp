#include <iostream>

#include "Math.h"
#include "Point.h"

int main() {
  Point a(0, 0, 0);
  Point b(1.5, 1.5, -1.5);
  Point t = a;
  while (t != b) {
    t = get_next_point_on_line(t, a, b);
    std::cout << t << "\n";
  }
  return 0;
}
