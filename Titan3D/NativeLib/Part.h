#pragma once

#include <memory>
#include <ostream>
#include <utility>
#include <vector>

#include "Callback.h"
#include "Facet.h"
#include "Fill.h"
#include "Matrix3D.h"
#include "Vector.h"

class Part {
 public:
  static std::unique_ptr<Part> make_Part(
      const std::string& filename, double scaling,
      Callback::AllocateMatrix matrix_cb, Callback::UpdateProgress progress_cb);
  static std::unique_ptr<Part> make_shell_Part(
      const std::vector<Facet>& facets, Callback::AllocateMatrix matrix_cb,
      Callback::UpdateProgress progress_cb);
  void fill_Part(Callback::UpdateProgress progress_cb);
  const Matrix3D<Fill, true>& matrix() const { return matrix_; }

 private:
  Part(Matrix3D<Fill, true>&& matrix, Vector&& origin) :
      matrix_(std::move(matrix)),
      origin_(std::move(origin)) {}

  static void update_element(Matrix3D<Fill, true>& matrix, const Vector& p,
                             const Vector& origin, Fill value);

  void fill_element(Fill in_fill, Fill& out_fill, FillState& state);

  Matrix3D<Fill, true> matrix_;
  Vector origin_;
};

std::ostream& operator<<(std::ostream& out, const Part& part);