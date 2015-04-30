#ifndef STL_PARSER_H
#define STL_PARSER_H

#include <fstream>
#include <memory>
#include <set>
#include <string>
#include <utility>

#include "Facet.h"
#include "Units.h"

class STLParser {
 public:
  static std::unique_ptr<STLParser> make_STLParser(
      const std::string& filename, Units units);
  virtual Facet read_facet() = 0;
  std::set<Facet> read_all_facets();

 protected:
  STLParser(std::ifstream&& fs, Units units) :
    fs_(std::move(fs)), units_(units), eof_(false) {}
  std::ifstream fs_;
  const Units units_;
  bool eof_;
};

class ASCIISTLParser : public STLParser {
 public:
  ASCIISTLParser(std::ifstream&& fs, Units units) :
    STLParser(std::move(fs), units) {}

  virtual Facet read_facet() final;
};

class BinarySTLParser : public STLParser {
 public:
  BinarySTLParser(std::ifstream&& fs, Units units, const int length) :
    STLParser(std::move(fs), units), length_(length) {}
  virtual Facet read_facet() final;

 private:
  const int length_;
};

#endif // STL_PARSER_H
