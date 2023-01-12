// @copyright c_la_vie 2022
#ifndef HEATDISTSIMULATION_HPP
#define HEATDISTSIMULATION_HPP

#include <math.h>
#include <omp.h>
#include <float.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "Log.hpp"

#define time_t std::chrono::time_point<std::chrono::system_clock>

struct heatPlate {
  // Plate distinguishers
  std::string plateName;
  int64_t heatPlateNumber;
  // Dimensions of the matrix
  int64_t rows;
  int64_t cols;
  // Time in seconds that marks each stage (generation) change
  double deltaT = 0;
  //  The thermal diffusivity set based on input
  double thermalDiffusivity = 0;
  // The cell dimension set based on input
  double cellDimension = 0;
  // The epsilon set based on input
  double epsilon = 0;
  // When temperature changes are lower than epsilon, simulation stops
  // Matrix of temperatures representing the plate
  std::vector<std::vector<double>> matrix;
  // An auxiliary matrix to store the state of the plate after an update
  std::vector<std::vector<double>> auxMatrix;
  // The starting time of the simulation
  double timeElapsed = 0;
  // Current generation of the simulation
  int64_t generation = 0;
  // the matrix with the current state of the simulation
  std::vector<std::vector<double>> currentMatrix;
  // The matrix that will hold the next generation
  std::vector<std::vector<double>> nextMatrix;
  // Constructor
  heatPlate(double epsilon, double thermalDiffusivity, double cellDimension,
            double deltaT);
  // Constructor with parameters
  heatPlate(std::string path, std::vector<std::string> parameters,
            int64_t heatPlateNumber);
  // Simple default constructor
  heatPlate();

  // Destructor, free of responsabilities
  ~heatPlate() {}

  /**
  * @brief Sets plate standard attributes read on a job
  * @param rows Amount of rows to be added to the matrix
  * @param cols Amount of cols to be added to the matrix
  */
  void setHeatPlateAtributes(size_t rows, size_t cols);

  /**
  * @brief Steps the simulation by one generation
  * @return the maximum temperature change in the generation
  */
  double updateGeneration();
  /**
  * @brief Updates a single cell of one generation
  * @param row The row of the cell to be updated
  * @param col The col of the cell to be updated
  * @return The temperature change in the cell
  */
  double updateCell(size_t row, size_t col);
};
/** 
* @brief Function that simulates the heat distribution in a plate
* @param heatPl A heat plate to be simulated
*/ 
void simulate(heatPlate* heatPl);
/**
* @brief Function that runs a simulation given a job file name
* @param jobFileName The name of the job file
*/
void runSimulation(std::string jobFileName);

#endif  // HEATDISTSIMULATION_HPP
