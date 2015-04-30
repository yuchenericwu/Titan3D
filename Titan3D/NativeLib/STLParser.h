#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Callback.h"
#include "Facet.h"

class STLParser {
 public:
  static std::unique_ptr<STLParser> make_STLParser(
      const std::string& filename, double scaling);
  std::vector<Facet> read_all_facets(Callback::UpdateProgress progress_cb);

 protected:
  STLParser(std::ifstream&& fs, double scaling, const size_t size) :
      fs_(std::move(fs)), scaling_(scaling), size_(size), eof_(false) {}
  virtual Facet read_facet() = 0;

  std::ifstream fs_;
  const double scaling_;
  const int size_;
  bool eof_;
};

class ASCIISTLParser : public STLParser {
 public:
  ASCIISTLParser(std::ifstream&& fs, double scaling, const size_t size) :
      STLParser(std::move(fs), scaling, size) {}
 private:
  virtual Facet read_facet() final;
};

class BinarySTLParser : public STLParser {
 public:
  BinarySTLParser(std::ifstream&& fs, double scaling, const size_t size) :
      STLParser(std::move(fs), scaling, size) {}
 private:
  virtual Facet read_facet() final;
};
