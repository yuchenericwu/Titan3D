#include "CppCliPart.h"

#include <msclr\marshal_cppstd.h>
#include <msclr\marshal_windows.h>
#include <string>

#include <exception>

#include "..\NativeLib\Callback.h"
#include "..\NativeLib\Part.h"
#include "CppCliFill.h"
#include "CppCliMatrix3D.h"

namespace CppCliDll {

  CppCliPart::CppCliPart(System::String^ filename, double scaling,
                         UpdateProgressDelegate^ progress_cb) {
    using System::IntPtr;
    using System::Runtime::InteropServices::GCHandle;
    using System::Runtime::InteropServices::Marshal;

    try {
      std::string unmanaged_filename =
          msclr::interop::marshal_as<std::string>(filename);
      AllocateMatrixDelegate^ matrix_cb = gcnew AllocateMatrixDelegate(
        this, &CppCliDll::CppCliPart::AllocateMatrix);
      GCHandle gch = GCHandle::Alloc(matrix_cb);
      IntPtr matrix_cbPtr = Marshal::GetFunctionPointerForDelegate(matrix_cb);
      IntPtr progress_cbPtr = Marshal::GetFunctionPointerForDelegate(progress_cb);

      System::GC::Collect();
      m_actualPart_ = Part::make_Part(
        unmanaged_filename, scaling,
        static_cast<Callback::AllocateMatrix>(matrix_cbPtr.ToPointer()),
        static_cast<Callback::UpdateProgress>(progress_cbPtr.ToPointer())).release();
      gch.Free();
    } catch (const std::exception& ex) {
      throw gcnew System::Exception(gcnew System::String(ex.what()));
    }
  }

  CppCliMatrix3D^ CppCliPart::matrix() {
    return matrix_;
  }

  Fill* CppCliPart::AllocateMatrix(
      size_t size_x, size_t size_y, size_t size_z) {
    matrix_ = gcnew CppCliMatrix3D(size_x, size_y, size_z);
    return matrix_->raw_data();
  }

} // namespace CppCliDll