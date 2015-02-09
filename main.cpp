#include <fstream>
#include <iostream>
#include <memory>
#include <utility>

#include "Facet.h"
#include "Part.h"
#include "STLParser.h"
#include "Units.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "Missing file argument.\n";
    return -1;
  }

  std::unique_ptr<STLParser> parser =
    STLParser::make_STLParser(argv[1], Units::MM);
  if (!parser) {
    std::cout << "Cannot parse STL file.\n";
    return -1;
  }

  std::vector<Facet> facets;
  while (true) {
    Facet facet = parser->read_facet();
    if (!facet.is_valid())
      break;
    facets.push_back(std::move(facet));
  }

  std::unique_ptr<Part> part = Part::make_Part(facets);
  if (!part) {
    std::cout << "Part parsing failed.\n";
    return -1;
  }
  std::ofstream fout("part_matrix.txt");
  fout << *part;

  return 0;
}
