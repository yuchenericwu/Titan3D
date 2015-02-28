#include "Facet.h"

#include <cassert>
#include <ostream>
#include <sstream>
#include <string>

#include <boost/crc.hpp>

#include "Constants.h"
#include "Vector.h"
#include "Units.h"

Facet::Facet(const VectorBase& n, const VectorBase v[3], Units units) :
    checksum_(0) {
  if (units == Units::MM) {
    normal_ = n;
    for (int ii = 0; ii < 3; ++ii)
      vertices_[ii] = v[ii];
  } else if (units == Units::IN) {
    normal_ = n * Constants::IN_TO_MM;
    for (int ii = 0; ii < 3; ++ii)
      vertices_[ii] = v[ii] * Constants::IN_TO_MM;
  } else {
    assert(0 && "Unrecognized unit.");
  }

  // After rounding, Facet has become invalid. No need to calculate checksum.
  if (normal_ == Vector() || vertices_[0] == vertices_[1] ||
      vertices_[0] == vertices_[2] || vertices_[1] == vertices_[2])
    return;
  
  std::stringstream ss;
  ss << *this;
  std::string facet_str = ss.str();
  boost::crc_32_type checksum;
  checksum.process_bytes(facet_str.c_str(), facet_str.size());
  checksum_ = static_cast<uint32_t>(checksum.checksum());
}

bool operator<(const Facet& lhs, const Facet& rhs) {
  return lhs.checksum() < rhs.checksum();
}

std::ostream& operator<<(std::ostream& out, const Facet& facet) {
  const Vector& normal = facet.normal();
  const Vector* vertices = facet.vertices();

  out << "n: (" << normal.x() << "," << normal.y() << "," << normal.z()
      << "), v: ";
  for (int ii = 0; ii < 3; ++ii) {
    out << "(" << vertices[ii].x() << "," << vertices[ii].y() << ","
        << vertices[ii].z() << ")";
    if (ii < 2)
      out << ", ";
  }
  out << "\n";
  return out;
}
