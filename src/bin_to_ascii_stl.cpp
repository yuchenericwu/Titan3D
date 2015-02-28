#include <fstream>
#include <iostream>
#include <memory>
#include <utility>

#include "Facet.h"
#include "STLParser.h"
#include "STLWriter.h"
#include "Units.h"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "Missing file arguments.\n";
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

  std::ofstream out_file(argv[2]);
  write_ascii_stl(out_file, facets);

  return 0;
}
