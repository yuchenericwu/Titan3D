#ifndef STL_WRITER_H
#define STL_WRITER_H

#include <ostream>
#include <string>
#include <vector>

#include "Facet.h"
#include "Vector.h"

void write_ascii_stl(std::ostream& out, const std::vector<Facet>& facets,
                     const std::string& part_name = "part_name") {
  out << "solid " << part_name << "\n";
  for (const Facet& facet : facets) {
    const Vector& normal = facet.normal();
    const Vector* vertices = facet.vertices();

    out << "  facet normal " << normal.x() << " " << normal.y() << " "
        << normal.z() << "\n    outer loop\n";
    for (int ii = 0; ii < 3; ++ii)
      out << "      vertex " << vertices[ii].x() << " " << vertices[ii].y()
          << " " << vertices[ii].z() << "\n";
    out << "    endloop\n  endfacet\n";
  }
  out << "endsolid " << part_name << "\n";
}

#endif
