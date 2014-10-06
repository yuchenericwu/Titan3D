#include <iostream>
#include <memory>
#include <utility>

#include "Facet.h"
#include "Part.h"
#include "STLParser.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "Missing file argument.\n";
    return -1;
  }

  std::unique_ptr<STLParser> parser =
    STLParser::make_STLParser(argv[1]);
  if (!parser) {
    std::cout << "Cannot parse STL file.\n";
    return -1;
  }

  Part part;
  while (true) {
    Facet facet = parser->read_facet();
    if (!facet.is_valid())
      break;
    part.add_facet(std::move(facet));
  }

  std::cout << part;

  return 0;
}
