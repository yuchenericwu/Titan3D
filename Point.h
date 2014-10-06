#ifndef POINT_H
#define POINT_H

#include <utility>

struct PointBase {
  PointBase() : x(0), y(0), z(0) {}
  PointBase(float x, float y, float z) : x(x), y(y), z(z) {}
  float x, y, z;
};

struct Point : PointBase {
  template<typename... Args> Point(Args&&... args) : 
    PointBase(std::forward<Args>(args)...) {}
  bool is_origin() const { return !x || !y || !z; }
};

struct Vector : PointBase {
  template<typename... Args> Vector(Args&&... args) : 
    PointBase(std::forward<Args>(args)...) {}
  bool is_valid() const { return x || y || z; }
};

#endif // POINT_H
