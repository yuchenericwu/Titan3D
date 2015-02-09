#ifndef STL_PARSER_H
#define STL_PARSER_H

#include <fstream>
#include <memory>
#include <string>
#include <utility>

#include "Facet.h"
#include "Point.h"
#include "Units.h"

class STLParser {
 public:
  static std::unique_ptr<STLParser> make_STLParser(
      const std::string& filename, Units units);
  virtual Facet read_facet() = 0;
 protected:
  STLParser(std::ifstream&& fs, Units units) :
    fs_(std::move(fs)), units_(units) {}
  std::ifstream fs_;
  const Units units_;
};

class ASCIISTLParser : public STLParser {
 public:
  ASCIISTLParser(std::ifstream&& fs, Units units) :
    STLParser(std::move(fs), units) {}

  virtual Facet read_facet() final {
    std::string facet, normal, outer, loop, vertex, endloop, endfacet;
    float x, y, z;
    fs_ >> facet >> normal >> x >> y >> z >> outer >> loop;
    if (facet != "facet" || normal != "normal" ||
        outer != "outer" || loop != "loop")
      return Facet();
    Vector n(x, y, z);

    Point v[3];
    for (int ii = 0; ii < 3; ++ii) {
      fs_ >> vertex >> x >> y >> z;
      if (vertex != "vertex")
        return Facet();
      v[ii] = Point(x, y, z);
    }

    fs_ >> endloop >> endfacet;
    if (endloop != "endloop" || endfacet != "endfacet")
      return Facet();

    return Facet(n, v, units_);
  }
};

class BinarySTLParser : public STLParser {
 public:
  BinarySTLParser(std::ifstream&& fs, Units units, const int length) :
    STLParser(std::move(fs), units), length_(length) {}

  virtual Facet read_facet() final {
    if (fs_.tellg() > length_ - 50)
      return Facet();
    char buffer[3][4];
    float x, y, z;

    for (int ii = 0; ii < 3; ++ii)
      fs_.read(buffer[ii], 4);
    x = *reinterpret_cast<float*>(buffer[0]);
    y = *reinterpret_cast<float*>(buffer[1]);
    z = *reinterpret_cast<float*>(buffer[2]);
    Vector n(x, y, z);
    
    Point v[3];
    for (int ii = 0; ii < 3; ++ii) {
      for (int jj = 0; jj < 3; ++jj)
        fs_.read(buffer[jj], 4);
      x = *reinterpret_cast<float*>(buffer[0]);
      y = *reinterpret_cast<float*>(buffer[1]);
      z = *reinterpret_cast<float*>(buffer[2]);
      v[ii] = Point(x, y, z);
    }
    fs_.read(buffer[0], 2);

    return Facet(n, v, units_);
  }
 private:
  const int length_;
};

std::unique_ptr<STLParser> STLParser::make_STLParser(
    const std::string& filename, Units units) {
  // Try opening file in ASCII format.
  std::ifstream fs;
  fs.open(filename);
  if (!fs.is_open())
    return nullptr;

  std::string solid, name;
  fs >> solid >> name;
  if (solid == "solid")
    return std::unique_ptr<ASCIISTLParser>(new ASCIISTLParser(
          std::move(fs), units));

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

#endif // STL_PARSER_H
