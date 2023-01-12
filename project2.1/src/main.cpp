// @copyright c_la_vie 2022

#include <stdint.h>

#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "HeatDistSimulation.hpp"
#include "Distribution.hpp"

int main(int argc, char * argv[]) {
  // Open file
  std::string usage = "basic: bin/project2.1 <input file>\n";
  std::string usage2 = "MPI: mpiexec -np <#processes> --map-by core bin/project2.1 <inputFile> <#threads>\n"; // NOLINT
  std::string usage3 = "OMP: bin/project2.1 <inputFile> <#threads>\n";
  if (argc < 2) {
    std::cout << "USAGE: \n" << usage << usage2 << usage3;
    return 1;
  }
  Distribution::startDistribution(argc, argv);
return 0;
}
