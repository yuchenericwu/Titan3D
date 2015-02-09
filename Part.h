#ifndef PART_H
#define PART_H

#include <cmath>
#include <ostream>
#include <utility>
#include <vector>

#include "Constants.h"
#include "Facet.h"
#include "Fill.h"
#include "Math.h"
#include "Matrix3D.h"
#include "Point.h"

class Part {
 public:
  static std::unique_ptr<Part> make_Part(const std::vector<Facet>& facets) {
    double x_min = facets.front().get_x_min();
    double x_max = facets.front().get_x_max();
    double y_min = facets.front().get_y_min();
    double y_max = facets.front().get_y_max();
    double z_min = facets.front().get_z_min();
    double z_max = facets.front().get_z_max();

    // Update the mins and maxes
    for (const Facet& facet : facets) {
      x_min = std::min(x_min, facet.get_x_min());
      x_max = std::max(x_max, facet.get_x_max());
      y_min = std::min(y_min, facet.get_y_min());
      y_max = std::max(y_max, facet.get_y_max());
      z_min = std::min(z_min, facet.get_z_min());
      z_max = std::max(z_max, facet.get_z_max());
    }

    // 1 extra element added for inclusivity, and 
    // 2 extra elements added for buffer space.
    double size_x_f = std::ceil((x_max - x_min) / Constants::PRECISION) + 3;
    double size_y_f = std::ceil((y_max - y_min) / Constants::PRECISION) + 3;
    double size_z_f = std::ceil((z_max - z_min) / Constants::PRECISION) + 3;
    if (size_x_f > UINT64_MAX || size_y_f > UINT64_MAX || size_z_f > UINT64_MAX)
      return nullptr;

    size_t size_x = size_t(size_x_f);
    size_t size_y = size_t(size_y_f);
    size_t size_z = size_t(size_z_f);
    Matrix3D<Fill> matrix(size_x, size_y, size_z);
    Point origin(x_min, y_min, z_min);
    Point extent(x_max, y_max, z_max);

    // Create transitional matrix
    for (const Facet& facet : facets) {
      const Point* vertices = facet.vertices();
      Point lineIt1 = vertices[0];
      Point lineIt2 = vertices[0];

      // Loops through points on the triangle bounded by its three vertices,
      // inclusively.
      while (true) {
        //Vector normal = get_point_above_surface(lineIt1, lineIt2);
        Vector normal = get_next_point_on_line(Point(), Point(),
                                               Point() + facet.normal());
        Point pointIt = lineIt1;

        // Loops through points on the line connecting lineIt1 and lineIt2,
        // inclusively.
        while (true) {
          update_element(matrix, pointIt, origin, Fill::INNER_EDGE);
          update_element(matrix, pointIt + normal, origin, Fill::OUTER_EDGE);

          // Last point has been updated, exit.
          if (pointIt == lineIt2)
            break;
          pointIt = get_next_point_on_line(pointIt, lineIt1, lineIt2);
        }

        // Last line has been updated, exit.
        if (lineIt1 == vertices[1] && lineIt2 == vertices[2])
          break;
        lineIt1 = get_next_point_on_line(lineIt1, vertices[0], vertices[1]);
        lineIt2 = get_next_point_on_line(lineIt2, vertices[0], vertices[2]);
      }
    }

    // Create final matrix
    for (size_t zz = 0; zz < size_z; ++zz) {
      for (size_t yy = 0; yy < size_y; ++yy) {
        FillState state = FillState::EMPTY;
        for (size_t xx = 0; xx < size_x; ++xx) {
          Fill& fill = matrix(xx, yy, zz);
          if (fill == Fill::SOLID)
            return nullptr;
          switch (state) {
            case FillState::EMPTY:
              if (fill == Fill::OUTER_EDGE) {
                fill = Fill::EMPTY;
                state = FillState::FILL_START;
              } else if (fill != Fill::EMPTY) {
                return nullptr;
              }
              break;
            case FillState::FILL_START:
              if (fill == Fill::INNER_EDGE) {
                fill = Fill::SOLID;
                state = FillState::FILLING;
              } else if (fill == Fill::OUTER_EDGE) {
                fill = Fill::EMPTY;
              } else { // fill == Fill::EMPTY
                state = FillState::EMPTY;
              }
              break;
            case FillState::FILLING:
              if (fill == Fill::EMPTY) {
                fill = Fill::SOLID;
              } else if (fill == Fill::INNER_EDGE) {
                fill = Fill::SOLID;
                state = FillState::FILL_END;
              } else { // fill == Fill::OUTER_EDGE
                fill = Fill::EMPTY;
                state = FillState::EMPTY;
              }
              break;
            case FillState::FILL_END:
              if (fill == Fill::INNER_EDGE) {
                fill = Fill::SOLID;
              } else { // fill == Fill::OUTER_EDGE || Fill::EMPTY
                fill = Fill::EMPTY;
                state = FillState::EMPTY;
              }
              break;
          }
        }
      }
    }

    return std::unique_ptr<Part>(new Part(
          std::move(matrix), std::move(origin), std::move(extent)));
  }

  const Matrix3D<Fill>& matrix() const {
    return matrix_;
  }

 private:
  Part(Matrix3D<Fill>&& matrix, Point&& origin, Point&& extent) :
      matrix_(std::move(matrix)),
      origin_(std::move(origin)),
      extent_(std::move(extent)) {}

  static void update_element(Matrix3D<Fill>& matrix, const Point& p,
                             const Point& origin, Fill value) {
    size_t x_index = (p.x() - origin.x()) / Constants::PRECISION + 1;
    size_t y_index = (p.y() - origin.y()) / Constants::PRECISION + 1;
    size_t z_index = (p.z() - origin.z()) / Constants::PRECISION + 1;
    Fill& fill = matrix(x_index, y_index, z_index);
    if (fill != Fill::INNER_EDGE)
      fill = value;
  }

  friend std::ostream& operator<<(std::ostream& out, const Part& part);
 private:
  Matrix3D<Fill> matrix_;
  Point origin_;
  Point extent_;
};

std::ostream& operator<<(std::ostream& out, const Part& part) {
  const Matrix3D<Fill>& matrix = part.matrix();
  for (auto fill : matrix) {
    char c;
    switch (fill) {
      case Fill::EMPTY:
        c = '0';
        break;
      case Fill::SOLID:
        c = '1';
        break;
      default:
        std::cerr << "Error: Invalid Fill encountered.\n";
    }
    out << c << ",";
  }
  return out;
}

#endif // PART_H
