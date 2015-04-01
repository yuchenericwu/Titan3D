#pragma once

#include "..\NativeLib\Fill.h"
#include "..\NativeLib\Part.h"
#include "CppCliMatrix3D.h"

namespace CppCliDll {

  public delegate Fill* AllocateMatrixDelegate(
      size_t size_x, size_t size_y, size_t size_z);
  public delegate void UpdateProgressDelegate(double progress, int status_enum);

  public ref class CppCliPart {
  public:
    CppCliPart(System::String^ filename, double scaling,
               UpdateProgressDelegate^ callback);
    CppCliMatrix3D^ matrix();

  private:
    Fill* AllocateMatrix(size_t size_x, size_t size_y, size_t size_z);

    Part* m_actualPart_;
    CppCliMatrix3D^ matrix_;
  };

} // namespace CppCliDll