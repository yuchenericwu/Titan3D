#ifndef PART_H
#define PART_H

#include <memory>
#include <ostream>
#include <set>
#include <utility>

#include "Facet.h"
#include "Fill.h"
#include "Matrix3D.h"
#include "Vector.h"

class Part {
 public:
  static std::unique_ptr<Part> make_Part(const std::string& filename,
                                         Units units);
  static std::unique_ptr<Part> make_shell_Part(const std::set<Facet>& facets);
  void fill_Part();
  const Matrix3D<Fill>& matrix() const { return matrix_; }

 private:
  Part(Matrix3D<Fill>&& matrix, Vector&& origin, Vector&& extent) :
      matrix_(std::move(matrix)),
      origin_(std::move(origin)),
      extent_(std::move(extent)) {}

  static void update_element(Matrix3D<Fill>& matrix, const Vector& p,
                             const Vector& origin, Fill value);

  void fill_element(Fill in_fill, Fill& out_fill, FillState& state);

  Matrix3D<Fill> matrix_;
  Vector origin_;
  Vector extent_;
};

std::ostream& operator<<(std::ostream& out, const Part& part);

#endif // PART_H
