#ifndef STL_PARSER_H
#define STL_PARSER_H

#include <fstream>
#include <memory>
#include <string>
#include <utility>

#include "Facet.h"
#include "Point.h"

class STLParser {
 public:
  static std::unique_ptr<STLParser> make_STLParser(const std::string& filename);
  virtual Facet read_facet() = 0;
 protected:
  STLParser(std::ifstream&& fs) : fs(std::move(fs)) {}
  std::ifstream fs;
};

class ASCIISTLParser : public STLParser {
 public:
  ASCIISTLParser(std::ifstream&& fs, const std::string& name) :
    STLParser(std::move(fs)),
    name(name) {}

  virtual Facet read_facet() final {
    std::string facet, normal, outer, loop, vertex, endloop, endfacet;
    Vector n;
    Point v[3];
    fs >> facet >> normal >> n.x >> n.y >> n.z >> outer >> loop;
    if (facet != "facet" || normal != "normal" ||
        outer != "outer" || loop != "loop")
      return Facet();

    for (int ii = 0; ii < 3; ++ii) {
      fs >> vertex >> v[ii].x >> v[ii].y >> v[ii].z;
      if (vertex != "vertex")
        return Facet();
    }

    fs >> endloop >> endfacet;
    if (endloop != "endloop" || endfacet != "endfacet")
      return Facet();

    return Facet(n, v);
  }
 private:
  const std::string name;
};

class BinarySTLParser : public STLParser {
 public:
  BinarySTLParser(std::ifstream&& fs, const int length) :
    STLParser(std::move(fs)), length(length) {}
  virtual Facet read_facet() final {
    if (fs.tellg() > length - 50)
      return Facet();
    char buffer[3][4];
    Vector n;
    Point v[3];

    for (int ii = 0; ii < 3; ++ii)
      fs.read(buffer[ii], 4);
    n.x = *reinterpret_cast<float*>(buffer[0]);
    n.y = *reinterpret_cast<float*>(buffer[1]);
    n.z = *reinterpret_cast<float*>(buffer[2]);
    
    for (int ii = 0; ii < 3; ++ii) {
      for (int jj = 0; jj < 3; ++jj)
        fs.read(buffer[jj], 4);
      v[ii].x = *reinterpret_cast<float*>(buffer[0]);
      v[ii].y = *reinterpret_cast<float*>(buffer[1]);
      v[ii].z = *reinterpret_cast<float*>(buffer[2]);
    }
    fs.read(buffer[0], 2);

    return Facet(n, v);
  }
 private:
  const int length;
};

std::unique_ptr<STLParser> STLParser::make_STLParser(
    const std::string& filename) {
  // Try opening file in ASCII format.
  std::ifstream fs;
  fs.open(filename);
  if (!fs.is_open())
    return nullptr;

  std::string solid, name;
  fs >> solid >> name;
  if (solid == "solid")
    return std::unique_ptr<ASCIISTLParser>(new ASCIISTLParser(
          std::move(fs), name));

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
        std::move(fs), length));
}

#endif // STL_PARSER_H
