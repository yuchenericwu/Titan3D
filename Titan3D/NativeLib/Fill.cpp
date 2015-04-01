#include "StdAfx.h"
#include "Fill.h"

Fill operator&&(Fill lhs, Fill rhs) {
  return (lhs == Fill::SOLID && rhs == Fill::SOLID) ? Fill::SOLID
                                                    : Fill::EMPTY;
}
