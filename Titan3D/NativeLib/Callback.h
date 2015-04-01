#pragma once

#include "Fill.h"

namespace Callback{
  typedef Fill* (__stdcall *AllocateMatrix)(size_t, size_t, size_t);
  typedef void (__stdcall *UpdateProgress)(double, int);
}