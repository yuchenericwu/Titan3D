#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <utility>

#include "Facet.h"
#include "Part.h"
#include "STLParser.h"
#include "Units.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Missing file argument.\n";
    return -1;
  }

  std::unique_ptr<STLParser> parser =
    STLParser::make_STLParser(argv[1], Units::MM);
  if (!parser) {
    std::cerr << "Cannot parse STL file.\n";
    return -1;
  }

  std::set<Facet> facets = parser->read_all_facets();
  std::unique_ptr<Part> part = Part::make_shell_Part(facets);
  if (!part) {
    std::cerr << "Part parsing failed.\n";
    return -1;
  }
  std::ofstream fout_temp("part_matrix_temp.txt");
  fout_temp << *part;
  part->fill_Part();
  std::ofstream fout("part_matrix.txt");
  fout << *part;

  return 0;
}
