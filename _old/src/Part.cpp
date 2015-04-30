#include "Part.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <ostream>
#include <set>
#include <utility>

#include "Constants.h"
#include "Facet.h"
#include "Fill.h"
#include "Math.h"
#include "Matrix3D.h"
#include "STLParser.h"
#include "Vector.h"

std::unique_ptr<Part> Part::make_Part(const std::string& filename,
                                      Units units) {
  std::unique_ptr<STLParser> parser =
    STLParser::make_STLParser(filename, Units::MM);
  if (!parser) {
    std::cerr << "Cannot parse STL file.\n";
    return nullptr;
  }

  std::set<Facet> facets = parser->read_all_facets();
  std::unique_ptr<Part> part = make_shell_Part(facets);
  if (!part) {
    std::cerr << "Part parsing failed.\n";
    return nullptr;
  }
  part->fill_Part();
  return part;
}

std::unique_ptr<Part> Part::make_shell_Part(const std::set<Facet>& facets) {
  double x_min = std::numeric_limits<double>::max();
  double y_min = std::numeric_limits<double>::max();
  double z_min = std::numeric_limits<double>::max();
  double x_max = std::numeric_limits<double>::lowest();
  double y_max = std::numeric_limits<double>::lowest();
  double z_max = std::numeric_limits<double>::lowest();

  // Update the mins and maxes
  for (const Facet& facet : facets) {
    x_min = std::min(x_min, facet.get_x_min());
    x_max = std::max(x_max, facet.get_x_max());
    y_min = std::min(y_min, facet.get_y_min());
    y_max = std::max(y_max, facet.get_y_max());
    z_min = std::min(z_min, facet.get_z_min());
    z_max = std::max(z_max, facet.get_z_max());
  }

  // 1 extra element added for inclusivity, and 2 extra elements in x-direction
  // for defining the outside edge of borders.
  double size_x_f = std::ceil((x_max - x_min) / Constants::PRECISION) + 3;
  double size_y_f = std::ceil((y_max - y_min) / Constants::PRECISION) + 1;
  double size_z_f = std::ceil((z_max - z_min) / Constants::PRECISION) + 1;
  if (size_x_f > std::numeric_limits<size_t>::max() ||
      size_y_f > std::numeric_limits<size_t>::max() ||
      size_z_f > std::numeric_limits<size_t>::max()) {
    std::cerr << "Error: Part is too large to store.\n";
    return nullptr;
  }

  size_t size_x = size_t(size_x_f);
  size_t size_y = size_t(size_y_f);
  size_t size_z = size_t(size_z_f);
  Matrix3D<Fill> matrix(size_x, size_y, size_z);
  Vector origin(x_min - Constants::PRECISION, y_min, z_min);
  Vector extent(x_max + Constants::PRECISION, y_max, z_max);

  // Create transitional matrix
  for (const Facet& facet : facets) {
    const Vector* vertices = facet.vertices();
    Vector lineIt1 = vertices[0];
    Vector lineIt2 = vertices[0];

    // Loop through points on the triangle bounded by its three vertices,
    // inclusively.
    while (true) {
      Vector pointIt = lineIt1;
      // Loop through points on the line connecting lineIt1 and lineIt2,
      // inclusively.
      while (true) {
        update_element(matrix, pointIt, origin, Fill::INNER_EDGE);

        // If normal vector points in negative x-direction, define the previous
        // pixel as an outside edge.
        if (facet.normal().x() < 0) {
          Vector outside = Vector(Constants::PRECISION, 0, 0);
          update_element(matrix, pointIt - outside, origin, Fill::OUTER_EDGE);
        // If normal vector points in positive x-direction, define the following
        // pixel as an outside edge.
        } else if (facet.normal().x() > 0) {
          Vector outside = Vector(Constants::PRECISION, 0, 0);
          update_element(matrix, pointIt + outside, origin, Fill::OUTER_EDGE);
        }

        // If the last point has been reached, exit.
        if (pointIt == lineIt2)
          break;
        pointIt = get_next_point_on_line(pointIt, lineIt1, lineIt2);
      }

      // If the last line has been updated, exit.
      if (lineIt1 == vertices[1] && lineIt2 == vertices[2])
        break;
      lineIt1 = get_next_point_on_line(lineIt1, vertices[0], vertices[1]);
      lineIt2 = get_next_point_on_line(lineIt2, vertices[0], vertices[2]);
    }
  }

  return std::unique_ptr<Part>(new Part(
        std::move(matrix), std::move(origin), std::move(extent)));
}

// Create finalized matrix.
void Part::fill_Part() {
  size_t size_x = matrix_.size_x();
  size_t size_y = matrix_.size_y();
  size_t size_z = matrix_.size_z();
  Matrix3D<Fill> left_pass_matrix(size_x, size_y, size_z);
  Matrix3D<Fill> right_pass_matrix(size_x, size_y, size_z);

  for (size_t zz = 0; zz < size_z; ++zz) {
    for (size_t yy = 0; yy < size_y; ++yy) {
      FillState left_pass_state = FillState::EMPTY;
      FillState right_pass_state = FillState::EMPTY;

      for (size_t xx = 0; xx < size_x; ++xx) {
        fill_element(matrix_(xx, yy, zz), left_pass_matrix(xx, yy, zz),
            left_pass_state);
        fill_element(matrix_(size_x-xx-1, yy, zz),
            right_pass_matrix(size_x-xx-1, yy, zz),
            right_pass_state);
      }
      for (size_t xx = 0; xx < size_x; ++xx)
        matrix_(xx, yy, zz) = 
          left_pass_matrix(xx, yy, zz) && right_pass_matrix(xx, yy, zz);
    }
  }
}

void Part::update_element(Matrix3D<Fill>& matrix, const Vector& p,
                                 const Vector& origin, Fill value) {
  size_t x_index = round((p.x() - origin.x()) / Constants::PRECISION, 1) + 1;
  size_t y_index = round((p.y() - origin.y()) / Constants::PRECISION, 1);
  size_t z_index = round((p.z() - origin.z()) / Constants::PRECISION, 1);
  Fill& fill = matrix(x_index, y_index, z_index);
  if (value != Fill::OUTER_EDGE || fill != Fill::INNER_EDGE)
    fill = value;
}

void Part::fill_element(Fill in_fill, Fill& out_fill, FillState& state) {
  switch (state) {
    case FillState::EMPTY:
      if (in_fill == Fill::OUTER_EDGE) {
        out_fill = Fill::EMPTY;
        state = FillState::FILL_START;
      } else if (in_fill != Fill::EMPTY) {
        out_fill = Fill::SOLID;
      }
      break;
    case FillState::FILL_START:
      if (in_fill == Fill::INNER_EDGE) {
        out_fill = Fill::SOLID;
        state = FillState::FILLING;
      } else { // in_fill == Fill::EMPTY
        state = FillState::EMPTY;
      }
      break;
    case FillState::FILLING:
      if (in_fill == Fill::EMPTY || in_fill == Fill::INNER_EDGE) {
        out_fill = Fill::SOLID;
      } else if (in_fill == Fill::OUTER_EDGE) {
        out_fill = Fill::EMPTY;
        state = FillState::FILL_END;
      }
      break;
    case FillState::FILL_END:
      if (in_fill == Fill::EMPTY) {
        state = FillState::EMPTY;
      } else if (in_fill == Fill::OUTER_EDGE) {
        in_fill = Fill::EMPTY;
        state = FillState::FILL_START;
      } else if (in_fill == Fill::INNER_EDGE) {
        out_fill = Fill::SOLID;
        state = FillState::FILLING;
      }
  }
}

std::ostream& operator<<(std::ostream& out, const Part& part) {
  const Matrix3D<Fill>& matrix = part.matrix();
  size_t size_x = matrix.size_x();
  size_t size_y = matrix.size_y();

  size_t ii = 0;
  for (auto fill : matrix) {
    char c;
    switch (fill) {
      case Fill::EMPTY:
        c = '0';
        break;
      case Fill::SOLID:
      case Fill::INNER_EDGE:
        c = '1';
        break;
      case Fill::OUTER_EDGE:
        c = '2';
        break;
      default:
        std::cerr << "Error: Invalid Fill encountered.\n";
    }
    out << c;
    
    ++ii;
    if (ii % size_x == 0)
      out << "\n";
    if (ii % (size_x*size_y) == 0)
      out << "\n";
  }
  return out;
}
