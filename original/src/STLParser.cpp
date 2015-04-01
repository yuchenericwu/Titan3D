#include "STLParser.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <utility>

#include "Facet.h"
#include "Vector.h"
#include "Units.h"

std::set<Facet> STLParser::read_all_facets() {
  std::set<Facet> facets;
  while (!eof_) {
    Facet facet = read_facet();
    if (facet.is_valid())
      facets.insert(std::move(facet));
    else if (!eof_)
      std::cerr << "Warning: Facet lost due to rounding error.\n";
  }
  return facets;
}
    
Facet ASCIISTLParser::read_facet() {
  if (eof_)
    return Facet();

  std::string facet, normal, outer, loop, vertex, endloop, endfacet;
  float x, y, z;
  fs_ >> facet >> normal >> x >> y >> z >> outer >> loop;
  if (facet != "facet" || normal != "normal" ||
      outer != "outer" || loop != "loop") {
    eof_ = true;
    return Facet();
  }
  VectorBase n(x, y, z);

  VectorBase v[3];
  for (int ii = 0; ii < 3; ++ii) {
    fs_ >> vertex >> x >> y >> z;
    if (vertex != "vertex") {
      eof_ = true;
      return Facet();
    }
    v[ii] = VectorBase(x, y, z);
  }

  fs_ >> endloop >> endfacet;
  if (endloop != "endloop" || endfacet != "endfacet") {
    eof_ = true;
    return Facet();
  }

  return Facet(n, v, units_);
}

Facet BinarySTLParser::read_facet() {
  if (eof_ || fs_.tellg() > length_ - 50) {
    eof_ = true;
    return Facet();
  }
  char buffer[3][4];
  float x, y, z;

  for (int ii = 0; ii < 3; ++ii)
    fs_.read(buffer[ii], 4);
  x = *reinterpret_cast<float*>(buffer[0]);
  y = *reinterpret_cast<float*>(buffer[1]);
  z = *reinterpret_cast<float*>(buffer[2]);
  Vector n(x, y, z);
  
  VectorBase v[3];
  for (int ii = 0; ii < 3; ++ii) {
    for (int jj = 0; jj < 3; ++jj)
      fs_.read(buffer[jj], 4);
    x = *reinterpret_cast<float*>(buffer[0]);
    y = *reinterpret_cast<float*>(buffer[1]);
    z = *reinterpret_cast<float*>(buffer[2]);
    v[ii] = VectorBase(x, y, z);
  }
  fs_.read(buffer[0], 2);

  return Facet(n, v, units_);
}

std::unique_ptr<STLParser> STLParser::make_STLParser(
    const std::string& filename, Units units) {
  // Try opening file in ASCII format.
  std::ifstream fs;
  fs.open(filename);
  if (!fs.is_open())
    return nullptr;

  std::string solid, name;
  fs >> solid >> name;
  if (solid == "solid") {
    auto cur_pos = fs.tellg();
    std::string facet;
    fs >> facet;
    if (facet == "facet") {
      fs.seekg(cur_pos);
      return std::unique_ptr<ASCIISTLParser>(new ASCIISTLParser(
            std::move(fs), units));
    }
  }

  // Try opening file in binary format.
  fs.close();
  fs.open(filename, std::ios_base::binary|std::ios_base::in);
  if (!fs.is_open())
    return nullptr;

  fs.seekg(0, fs.end);
  int length = fs.tellg();
  fs.seekg(80);
  char buffer[4];
  fs.read(buffer, 4);
  uint32_t num_triangles = *reinterpret_cast<uint32_t*>(buffer);
  if (num_triangles * 50 != length - 84)
    return nullptr;
  return std::unique_ptr<BinarySTLParser>(new BinarySTLParser(
        std::move(fs), units, length));
}
