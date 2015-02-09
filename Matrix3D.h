#ifndef MATRIX3D_H
#define MATRIX3D_H

#include <utility>
#include <vector>

template<typename T_>
class Matrix3D {
 public:
  Matrix3D(size_t size_x, size_t size_y, size_t size_z) :
      matrix_(size_x*size_y*size_z),
      size_x_(size_x), size_y_(size_y), size_z_(size_z) {}

  Matrix3D(Matrix3D<T_>&& other) :
      matrix_(std::move(other.matrix_)),
      size_x_(other.size_x_), size_y_(other.size_y_), size_z_(other.size_z_) {}

  T_& operator()(size_t xx, size_t yy, size_t zz) {
    return matrix_[zz*size_y_*size_x_ + yy*size_x_ + xx];
  }

  typename std::vector<T_>::const_iterator begin() const {
    return matrix_.begin();
  }
  
  typename std::vector<T_>::const_iterator end() const {
    return matrix_.end();
  }
 private:
  std::vector<T_> matrix_;
  const size_t size_x_, size_y_, size_z_;
};

#endif // MATRIX3D_H
