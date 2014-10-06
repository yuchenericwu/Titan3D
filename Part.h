#ifndef PART_H
#define PART_H

#include <ostream>
#include <utility>
#include <vector>

#include "Facet.h"

class Part {
 public:
  Part() : facets(), x_min(0), x_max(0), y_min(0), y_max(0), z_min(0), 
           z_max(0) {}

  Part(std::vector<Facet>&& facets) : facets(std::move(facets)) {
    x_min = facets.front().get_x_min();
    x_max = facets.front().get_x_max();
    y_min = facets.front().get_y_min();
    y_max = facets.front().get_y_max();
    z_min = facets.front().get_z_min();
    z_max = facets.front().get_z_max();
    
    for (Facet facet : facets) {
      x_min = std::min(x_min, facet.get_x_min());
      x_max = std::max(x_max, facet.get_x_max());
      y_min = std::min(y_min, facet.get_y_min());
      y_max = std::max(y_max, facet.get_y_max());
      z_min = std::min(z_min, facet.get_z_min());
      z_max = std::max(z_max, facet.get_z_max());
    }
  }

  void add_facet(Facet&& facet) {
    facets.push_back(std::move(facet));
    x_min = std::min(x_min, facet.get_x_min());
    x_max = std::max(x_max, facet.get_x_max());
    y_min = std::min(y_min, facet.get_y_min());
    y_max = std::max(y_max, facet.get_y_max());
    z_min = std::min(z_min, facet.get_z_min());
    z_max = std::max(z_max, facet.get_z_max());
  }

  friend std::ostream& operator<<(std::ostream& out, const Part& part);
 private:
  std::vector<Facet> facets;
  float x_min;
  float x_max;
  float y_min;
  float y_max;
  float z_min;
  float z_max;
};

std::ostream& operator<<(std::ostream& out, const Part& part) {
  out << "Part:\n" << "x_min: " << part.x_min << "\nx_max: " << part.x_max
    << "\ny_min: " << part.y_min << "\ny_max: " << part.y_max
    << "\nz_min: " << part.z_min << "\nz_max: " << part.z_max << "\n\n";
  for (Facet facet : part.facets)
    out << "Facet:\n" << facet << "\n";
  return out;
}

#endif // PART_H
