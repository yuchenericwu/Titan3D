#pragma once

#include <utility>
#include <vector>

template<typename T_, bool Managed_ = false>
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

  T_& operator()(size_t xx, size_t yy) {
    return matrix_[yy*size_x_ + xx];
  }

  typename std::vector<T_>::const_iterator begin() const {
    return matrix_.begin();
  }
  
  typename std::vector<T_>::const_iterator end() const {
    return matrix_.end();
  }

  const T_* data() const {
    return matrix_.data();
  }

  const size_t size_x() const { return size_x_; }
  const size_t size_y() const { return size_y_; }
  const size_t size_z() const { return size_z_; }
 private:
  std::vector<T_> matrix_;
  const size_t size_x_, size_y_, size_z_;
};

template<typename T_>
class Matrix3D<T_, true> {
public:
  Matrix3D(Callback::AllocateMatrix allocator, size_t size_x, size_t size_y,
           size_t size_z) :
    matrix_(allocator(size_x, size_y, size_z)),
    size_x_(size_x), size_y_(size_y), size_z_(size_z) {}

  Matrix3D(Matrix3D<T_, true>&& other) :
    matrix_(other.matrix_),
    size_x_(other.size_x_), size_y_(other.size_y_), size_z_(other.size_z_) {}

  T_& operator()(size_t xx, size_t yy, size_t zz) {
    return matrix_[zz*size_y_*size_x_ + yy*size_x_ + xx];
  }

  T_& operator()(size_t xx, size_t yy) {
    return matrix_[yy*size_x_ + xx];
  }

  const T_* begin() const {
    return matrix_;
  }

  const T_* end() const {
    return &matrix_[size_x_ * size_y_ * size_z_];
  }

  const size_t size_x() const { return size_x_; }
  const size_t size_y() const { return size_y_; }
  const size_t size_z() const { return size_z_; }
private:
  T_* matrix_;
  const size_t size_x_, size_y_, size_z_;
};