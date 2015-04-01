#include "StdAfx.h"
#include "STLParser.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "Exception.h"
#include "Facet.h"
#include "Vector.h"

std::vector<Facet> STLParser::read_all_facets(
    Callback::UpdateProgress progress_cb) {
  std::set<Facet> facets;
  while (!eof_) {
    Facet facet = read_facet();
    if (facet.is_valid())
      facets.insert(std::move(facet));

    if (progress_cb)
      progress_cb((double)fs_.tellg() / size_, Constants::PARSE);
  }
  return std::vector<Facet>(facets.begin(), facets.end());
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
  VectorBase n(x * Constants::X_SCALING, y, z);

  VectorBase v[3];
  for (int ii = 0; ii < 3; ++ii) {
    fs_ >> vertex >> x >> y >> z;
    if (vertex != "vertex") {
      eof_ = true;
      return Facet();
    }
    v[ii] = VectorBase(x * Constants::X_SCALING, y, z);
  }

  fs_ >> endloop >> endfacet;
  if (endloop != "endloop" || endfacet != "endfacet") {
    eof_ = true;
    return Facet();
  }

  return Facet(n, v, scaling_);
}

Facet BinarySTLParser::read_facet() {
  if (eof_ || fs_.tellg() > size_ - 50) {
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
  Vector n(x * Constants::X_SCALING, y, z);
  
  VectorBase v[3];
  for (int ii = 0; ii < 3; ++ii) {
    for (int jj = 0; jj < 3; ++jj)
      fs_.read(buffer[jj], 4);
    x = *reinterpret_cast<float*>(buffer[0]);
    y = *reinterpret_cast<float*>(buffer[1]);
    z = *reinterpret_cast<float*>(buffer[2]);
    v[ii] = VectorBase(x * Constants::X_SCALING, y, z);
  }
  fs_.read(buffer[0], 2);

  return Facet(n, v, scaling_);
}

std::unique_ptr<STLParser> STLParser::make_STLParser(
    const std::string& filename, double scaling) {
  // Try opening file and store its size.
  std::ifstream fs(filename, std::ifstream::in | std::ifstream::binary);
  if (!fs)
    throw Exception::InvalidFile("Invalid STL file: " + filename);
  fs.seekg(0, std::ios_base::end);
  size_t size = fs.tellg();
  fs.close();

  // Try opening file in ASCII format.
  fs.open(filename);
  std::string solid, name, facet;
  fs >> solid >> name >> facet;
  if (solid == "solid" && facet == "facet") {
    // Reopen file and reread header to ensure cursor is at correct location.
    // tellg() and seekg() does not work well in text mode on Windows.
    fs.close();
    fs.open(filename);
    fs >> solid >> name;
    return std::unique_ptr<ASCIISTLParser>(new ASCIISTLParser(
          std::move(fs), scaling, size));
  }
  fs.close();

  // Try opening file in binary format.
  fs.open(filename, std::ios_base::binary | std::ios_base::in);
  fs.seekg(80);
  char buffer[4];
  fs.read(buffer, 4);
  uint32_t num_triangles = *reinterpret_cast<uint32_t*>(buffer);
  if (num_triangles * 50 != size - 84)
    throw Exception::InvalidFile("Invalid STL file: " + filename);
  return std::unique_ptr<BinarySTLParser>(new BinarySTLParser(
        std::move(fs), scaling, size)); 
}
