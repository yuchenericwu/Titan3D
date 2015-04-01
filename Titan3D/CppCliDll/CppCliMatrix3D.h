#pragma once

#include <vector>
#include "..\NativeLib\Fill.h"
#include "..\NativeLib\Matrix3D.h"
#include "CppCliFill.h"

namespace CppCliDll {

  public ref class CppCliMatrix3D {
  public:
    CppCliMatrix3D(size_t size_x, size_t size_y, size_t size_z) :
        data_(gcnew array<CppCliFill>(size_x * size_y * size_z)),
        size_x_(size_x), size_y_(size_y), size_z_(size_z) {}
    CppCliMatrix3D(const CppCliMatrix3D% other) :
        data_(other.data_), size_x_(other.size_x_), size_y_(other.size_y_),
        size_z_(other.size_z_) {}

    array<CppCliFill>^ data() {
      return data_;
    }

    Fill* raw_data() {
      pin_ptr<CppCliFill> p_data = &data_[0];
      return reinterpret_cast<Fill*>(p_data);
    }

    size_t size_x() { return size_x_; }
    size_t size_y() { return size_y_; }
    size_t size_z() { return size_z_; }

  private:
    array<CppCliFill>^ data_;
    size_t size_x_;
    size_t size_y_;
    size_t size_z_;
  };

}