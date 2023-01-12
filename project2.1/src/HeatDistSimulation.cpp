// @copyright c_la_vie 2022
#include "HeatDistSimulation.hpp"
#include "fileManager.hpp"

heatPlate::heatPlate(double epsilon,
  double thermalDiffusivity, double cellDimension, double deltaT) {
  this->epsilon = epsilon;
  this->thermalDiffusivity = thermalDiffusivity;
  this->cellDimension = cellDimension;
  this->deltaT = deltaT;
}

heatPlate::heatPlate() {
  this->epsilon = 0;
  this->thermalDiffusivity = 0;
  this->cellDimension = 0;
  this->deltaT = 0;
  this->generation = 0;
  this->timeElapsed = 0;
  this->plateName = "";
  this->heatPlateNumber = 0;
  this->rows = 0;
  this->cols = 0;
}

heatPlate::heatPlate(std::string path, std::vector<std::string> parameters,
                     int64_t heatPlateNumber) {
  try {
    std::string platePath = path + parameters[0];
    this->deltaT = std::stod(parameters[1]);
    this->thermalDiffusivity = std::stod(parameters[2]);
    this->cellDimension = std::stod(parameters[3]);
    this->epsilon = std::stod(parameters[4]);
    FileManager::readMatrix(this, platePath);
    this->auxMatrix = this->matrix;
    this->plateName = FileManager::cleanExtension(platePath, ".bin");
    this->heatPlateNumber = heatPlateNumber;
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}


void heatPlate::setHeatPlateAtributes(size_t rows, size_t cols) {
  this->rows = rows;
  this->cols = cols;
  this->matrix = std::vector<std::vector<double>>(rows,
                                                  std::vector<double>(cols));
  this->auxMatrix = std::vector<std::vector<double>>(rows,
                                                     std::vector<double>(cols));
}


void runSimulation(std::string jobFileName) {
  try {
    // get all plates in the job file
    std::vector<std::string> plates = FileManager::getPlates(jobFileName);
    Log::Report report(jobFileName);
    std::string path = jobFileName.substr(0, jobFileName.find_last_of("/") + 1);
    std::cout << path << std::endl;
    // For each plate in the job file
    for (uint64_t plater = 0; plater < plates.size(); plater++) {
      // Create a heat plate struct
      std::string plateInfo = plates[plater];
      // tests/job001/job001.txt
      // tests/job001/ + plate001.bin
      std::vector<std::string> parameters =
                                FileManager::getSimulationParameters(plateInfo);
      heatPlate* heatPl = new heatPlate(path, parameters, plater);
      // and simulate it
      report.writeRow(parameters);
      simulate(heatPl);
      report.writeResult(heatPl->generation, heatPl->timeElapsed);
      std::string outputPath = heatPl->plateName + "-" + std::to_string(
                                                         heatPl->generation);
      Log::binaryMatrixWriter(outputPath, heatPl->currentMatrix);
      delete heatPl;
    }
    report.writeReport();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

void simulate(heatPlate* heatPl) {
  heatPl->currentMatrix = heatPl->matrix;
  heatPl->nextMatrix = heatPl->auxMatrix;
  double deltaDifference = 0.0;
  while ((deltaDifference = heatPl->updateGeneration()) > heatPl->epsilon) {}
  heatPl->timeElapsed = heatPl->generation * heatPl->deltaT;
}


double heatPlate::updateGeneration() {
  // The maximum temperature change detected in this generation
  double maxChange = DBL_MIN;
  // Update all rows and columns
  #pragma omp parallel for default(none) schedule(static) \
  collapse(2) reduction(max:maxChange)
  for (int64_t row = 1; row < this->rows - 1; ++row) {
    for (int64_t col = 1; col < this->cols - 1; ++col) {
      // Update cell from current matrix to the next matrix
      double change = updateCell(row, col);
      maxChange = change > maxChange ? change : maxChange;
    }  // end for
  }  // end for
  // The next matrix is enterely updated, it is now the current matrix
  // The current matrix will be used for the next generation. So swap them
  std::swap(this->currentMatrix, this->nextMatrix);
  // A new generation was completed so increment the generation counter
  this->generation = this->generation + 1;
  // Return the max temperature change detected in this generation
  return maxChange;
}

double heatPlate::updateCell(size_t row, size_t col) {
  // The new value is the average of the neighbors in the previous generation
  this->nextMatrix[row][col] =
    this->currentMatrix[row][col]+
    (((this->deltaT)*(this->thermalDiffusivity))/
    (pow(this->cellDimension, 2)))*
    (this->currentMatrix[row - 1][col]
    + this->currentMatrix[row][col + 1]
    + this->currentMatrix[row][col - 1]
    + this->currentMatrix[row + 1][col] - 4*this->currentMatrix[row][col]);
  // Returns the absolute value of the change in temperature
  return fabs(this->nextMatrix[row][col] - this->currentMatrix[row][col]);
}
