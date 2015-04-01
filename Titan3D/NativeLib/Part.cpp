#include "Stdafx.h"
#include "Part.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <ostream>
#include <sstream>
#include <thread>
#include <utility>
#include <vector>

#include "Constants.h"
#include "Exception.h"
#include "Facet.h"
#include "Fill.h"
#include "Math.h"
#include "Matrix3D.h"
#include "STLParser.h"
#include "Vector.h"

std::unique_ptr<Part> Part::make_Part(
    const std::string& filename, double scaling, Callback::AllocateMatrix matrix_cb,
    Callback::UpdateProgress progress_cb) {
  std::unique_ptr<STLParser> parser =
      STLParser::make_STLParser(filename, scaling);
  std::vector<Facet> facets = parser->read_all_facets(progress_cb);
  std::unique_ptr<Part> part = make_shell_Part(facets, matrix_cb, progress_cb);
  part->fill_Part(progress_cb);
  return part;
}

std::unique_ptr<Part> Part::make_shell_Part(
    const std::vector<Facet>& facets, Callback::AllocateMatrix matrix_cb,
    Callback::UpdateProgress progress_cb) {
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

  // 1 extra element added for inclusivity, and 2 extra elements in x- and
  // y- direction for defining the outside edge of borders.
  double size_x = std::ceil((x_max - x_min) / Constants::PRECISION) + 3;
  double size_y = std::ceil((y_max - y_min) / Constants::PRECISION) + 3;
  double size_z = std::ceil((z_max - z_min) / Constants::PRECISION) + 1;
  if (size_x > Constants::MAX_SIZE_X || size_y > Constants::MAX_SIZE_Y ||
      size_z > Constants::MAX_SIZE_Z) {
    std::ostringstream ss;
    ss << "Part is too large for print bed: " << x_max - x_min << "mm x "
       << y_max - y_min << "mm x " << z_max - z_min << "mm.\n"
       << "Max size is 50.8mm x 50.8mm x 44.8mm.";
    throw Exception::NumericBounds(ss.str());
  }

  Matrix3D<Fill, true> matrix(matrix_cb, size_x, size_y, size_z);
  Vector origin(x_min - Constants::PRECISION, y_min - Constants::PRECISION, z_min);

  size_t num_complete = 0;
  size_t num_facets = facets.size();

  // Create transitional matrix
  #pragma omp parallel for
  for (int ii = 0; ii < num_facets; ++ii) {
    const Facet& facet = facets[ii];

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
        Vector x_unit = Vector(Constants::PRECISION, 0, 0);
        Vector y_unit = Vector(0, Constants::PRECISION, 0);

        // If normal vector points in negative x-direction, define the previous
        // pixel as an outside edge.
        if (facet.normal().x() < 0)
          update_element(matrix, pointIt - x_unit, origin, Fill::OUTER_EDGE);
        // If normal vector points in positive x-direction, define the following
        // pixel as an outside edge.
        else if (facet.normal().x() > 0)
          update_element(matrix, pointIt + x_unit, origin, Fill::OUTER_EDGE);

        // If normal vector points in negative y-direction, define the previous
        // pixel as an outside edge.
        if (facet.normal().y() < 0)
          update_element(matrix, pointIt - y_unit, origin, Fill::OUTER_EDGE);
        // If normal vector points in positive y-direction, define the following
        // pixel as an outside edge.
        else if (facet.normal().y() > 0)
          update_element(matrix, pointIt + y_unit, origin, Fill::OUTER_EDGE);

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

    if (progress_cb) {
      #pragma omp atomic
      ++num_complete;
      progress_cb((double)num_complete / num_facets, Constants::SHELL);
    }
  }

  return std::unique_ptr<Part>(new Part(std::move(matrix), std::move(origin)));
}

// Create finalized matrix.
void Part::fill_Part(Callback::UpdateProgress progress_cb) {
  size_t size_x = matrix_.size_x();
  size_t size_y = matrix_.size_y();
  size_t size_z = matrix_.size_z();
  size_t higher_limit = std::max(size_x, size_y);

  size_t num_complete = 0;
  #pragma omp parallel for
  for (int zz = 0; zz < size_z; ++zz) {
    Matrix3D<Fill> left_pass_matrix(size_x, size_y, 1);
    Matrix3D<Fill> right_pass_matrix(size_x, size_y, 1);
    Matrix3D<Fill> up_pass_matrix(size_x, size_y, 1);
    Matrix3D<Fill> down_pass_matrix(size_x, size_y, 1);

    for (size_t outer_index = 0; outer_index < higher_limit; ++outer_index) {
      FillState left_pass_state = FillState::EMPTY;
      FillState right_pass_state = FillState::EMPTY;
      FillState up_pass_state = FillState::EMPTY;
      FillState down_pass_state = FillState::EMPTY;

      for (size_t inner_index = 0; inner_index < higher_limit; ++inner_index) {
        if (inner_index < size_x && outer_index < size_y) {
          fill_element(matrix_(inner_index, outer_index, zz), 
                       left_pass_matrix(inner_index, outer_index),
                      left_pass_state);
          fill_element(matrix_(size_x-inner_index-1, outer_index, zz),
                       right_pass_matrix(size_x-inner_index-1, outer_index),
                       right_pass_state);
        }
        if (inner_index < size_y && outer_index < size_x)  {
          fill_element(matrix_(outer_index, inner_index, zz),
                       up_pass_matrix(outer_index, inner_index),
                       up_pass_state);
          fill_element(matrix_(outer_index, size_y-inner_index-1, zz),
                       down_pass_matrix(outer_index, size_y-inner_index-1),
                       down_pass_state);
        }
      }
    }

    for (size_t yy = 0; yy < size_y; ++yy) {
      for (size_t xx = 0; xx < size_x; ++xx) {
        // Set element to solid if 2 or more pass matrices think it's solid.
        if (left_pass_matrix(xx, yy) == Fill::SOLID) {
          if (right_pass_matrix(xx, yy) == Fill::SOLID ||
              up_pass_matrix(xx, yy) == Fill::SOLID ||
              down_pass_matrix(xx, yy) == Fill::SOLID) {
            matrix_(xx, yy, zz) = Fill::SOLID;
          } else {
            matrix_(xx, yy, zz) = Fill::EMPTY;
          }
        } else if (right_pass_matrix(xx, yy) == Fill::SOLID) {
          if (up_pass_matrix(xx, yy) == Fill::SOLID ||
              down_pass_matrix(xx, yy) == Fill::SOLID) {
            matrix_(xx, yy, zz) = Fill::SOLID;
          } else {
            matrix_(xx, yy, zz) = Fill::EMPTY;
          }
        } else if (up_pass_matrix(xx, yy) == Fill::SOLID &&
                   down_pass_matrix(xx, yy) == Fill::SOLID) {
          matrix_(xx, yy, zz) = Fill::SOLID;
        } else {
          matrix_(xx, yy, zz) = Fill::EMPTY;
        }
      }
    }

    if (progress_cb) {
      #pragma omp atomic
      ++num_complete;
      progress_cb((double)num_complete / size_z, Constants::FILL);
    }
  }
}
void Part::update_element(Matrix3D<Fill, true>& matrix, const Vector& p,
                          const Vector& origin, Fill value) {
  size_t x_index = round((p.x() - origin.x()) / Constants::PRECISION, 1);
  size_t y_index = round((p.y() - origin.y()) / Constants::PRECISION, 1);
  size_t z_index = round((p.z() - origin.z()) / Constants::PRECISION, 1);
  Fill& fill = matrix(x_index, y_index, z_index);

  // Don't allow values of OUTER_EDGE to overwrite existing fills of INNER_EDGE.
  #pragma omp critical
  if (value != Fill::OUTER_EDGE || fill != Fill::INNER_EDGE)
    fill = value;
}

void Part::fill_element(Fill in_fill, Fill& out_fill, FillState& state) {
  out_fill = in_fill;
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
  const Matrix3D<Fill, true>& matrix = part.matrix();
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
