// @copyright c_la_vie 2022
#include "Distribution.hpp"
#include "HeatDistSimulation.hpp"

/**
* @brief Receove an input matrix and return a single vector with all values
* @param matrix to process
* @return vector with all values of matrix
*/
std::vector<double> mergeVector(std::vector<std::vector<double>> matrix) {
  std::vector<double> output;
  for (uint64_t vector = 0; vector < matrix.size(); ++vector) {
    std::vector<double> target = matrix[vector];
    for (uint64_t index = 0; index < target.size(); ++index) {
      double unit = target[index];
      output.push_back(unit);
    }  // end for
  }  // end for
  return output;
}

void Distribution::sendHeatPlate(heatPlate* heatPl,
                                int dest) {
  // Send the heat plate number
  MPI_Send(&heatPl->heatPlateNumber, 1, MPI_INT64_T, dest, 8, MPI_COMM_WORLD);
  // Send the number of rows
  MPI_Send(&heatPl->rows, 1, MPI_INT64_T , dest, 0, MPI_COMM_WORLD);
  // Send the number of cols
  MPI_Send(&heatPl->cols, 1, MPI_INT64_T, dest, 1, MPI_COMM_WORLD);
  // Send the delta t
  MPI_Send(&heatPl->deltaT, 1, MPI_DOUBLE, dest, 2, MPI_COMM_WORLD);
  // Send the thermal diffusivity
  MPI_Send(&heatPl->thermalDiffusivity, 1, MPI_DOUBLE, dest, 3,
            MPI_COMM_WORLD);
  // Send the cell dimension
  MPI_Send(&heatPl->cellDimension, 1, MPI_DOUBLE, dest, 4, MPI_COMM_WORLD);
  // Send the epsilon
  MPI_Send(&heatPl->epsilon, 1, MPI_DOUBLE, dest, 5, MPI_COMM_WORLD);
  // Send the size of the heat plate name
  uint64_t size = heatPl->plateName.size();
  MPI_Send(&size, 1, MPI_UINT64_T, dest, 6, MPI_COMM_WORLD);
  // Send the heat plate name
  MPI_Send(heatPl->plateName.data(), heatPl->plateName.size(),
            MPI_CHAR, dest, 7, MPI_COMM_WORLD);
  // Send the matrix size
  std::vector<double> input = mergeVector(heatPl->matrix);
  // Send the matrix
  MPI_Send(input.data(), input.size(), MPI_DOUBLE, dest, 7, MPI_COMM_WORLD);
}  // end if


void Distribution::receiveHeatPlate(heatPlate* heatPl) {
  // Receive the heat plate number
  MPI_Recv(&heatPl->heatPlateNumber, 1, MPI_INT64_T, 0, 8, MPI_COMM_WORLD,
           MPI_STATUS_IGNORE);
  // Receive number of rows
  MPI_Recv(&heatPl->rows, 1, MPI_INT64_T, 0, 0, MPI_COMM_WORLD,
           MPI_STATUS_IGNORE);
  // Receive number of cols
  MPI_Recv(&heatPl->cols, 1, MPI_INT64_T, 0, 1, MPI_COMM_WORLD,
           MPI_STATUS_IGNORE);
  // Receive the delta t
  MPI_Recv(&heatPl->deltaT, 1, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD,
           MPI_STATUS_IGNORE);
  // Receive the thermal diffusivity
  MPI_Recv(&heatPl->thermalDiffusivity, 1, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD,
           MPI_STATUS_IGNORE);
  // Receive cell dimensions
  MPI_Recv(&heatPl->cellDimension, 1, MPI_DOUBLE, 0, 4, MPI_COMM_WORLD,
           MPI_STATUS_IGNORE);
  // Receive epsilon
  MPI_Recv(&heatPl->epsilon, 1, MPI_DOUBLE, 0, 5, MPI_COMM_WORLD,
           MPI_STATUS_IGNORE);
  // Receive heat plate name size
  uint64_t nameSize = 0;
  MPI_Recv(&nameSize, 1, MPI_UINT64_T, 0, 6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  // Resize the string
  std::vector<char> plateName(nameSize);
  // Receive heat plate name
  MPI_Recv(&plateName[0], nameSize, MPI_CHAR, 0, 7, MPI_COMM_WORLD,
           MPI_STATUS_IGNORE);
  heatPl->plateName = std::string(plateName.begin(), plateName.end());
  // Receive matrix
  // Prepare a buffer to receive the matrix
  std::vector<double> buffer(heatPl->rows * heatPl->cols);
  // receive the matrix into the buffer
  MPI_Recv(&buffer[0], heatPl->rows * heatPl->cols, MPI_DOUBLE, 0, 7,
            MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  // Prepare the matrix
  heatPl->matrix = std::vector<std::vector<double>>(heatPl->rows,
                               std::vector<double>(heatPl->cols));
  // Fill the matrix with the buffer
  int64_t index = 0;
  for (int plater = 0; plater < heatPl->rows; plater++) {
    for (int j = 0; j < heatPl->cols; j++) {
      heatPl->matrix[plater][j] = buffer[index];
      index++;
    }
  }
  // Set the auxiliary matrix
  heatPl->auxMatrix = heatPl->matrix;
}

void Distribution::distributeHeatPlates(
  std::vector<heatPlate*>& plates, int processCount) {
    int64_t plateId = plates.size();  // The number of plates to be distributed
    int processRequestNumber = 0;  //  The id of the process to send the plate
    while (plateId > 0) {
      // Wait for a process to request a plate
      MPI_Recv(&processRequestNumber, 1, MPI_INT, MPI_ANY_SOURCE, 10,
      MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      // Tell the process that a plate is available
      MPI_Send(&plateId, 1, MPI_INT64_T, processRequestNumber, 11,
               MPI_COMM_WORLD);
      // Send the plate to the process
      sendHeatPlate(plates[plateId - 1], processRequestNumber);
      plateId--;
    }
    plateId = -1;
    for (int process = 1; process < processCount; process++) {
      MPI_Recv(&processRequestNumber, 1, MPI_INT, MPI_ANY_SOURCE, 10,
        MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      // send a message to all processes to stop waiting for plates
      MPI_Send(&plateId, 1, MPI_INT, processRequestNumber, 11,
      MPI_COMM_WORLD);
    }
}

void Distribution::simulateHeatPlates(std::vector<heatPlate*>& plates,
                                      int processNumber) {
  int plateId = 0;
  // Tell the master process that you are ready to receive a plate
  while (plateId != -1) {
    // Send a request to the master process
    MPI_Send(&processNumber, 1, MPI_INT, 0, 10, MPI_COMM_WORLD);
    // Wait for a plate to be available
    MPI_Recv(&plateId, 1, MPI_INT64_T, 0, 11, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    if (plateId != -1) {
      // Prepare a new plate
      heatPlate* heatPl = new heatPlate();
      // Receive the plate
      receiveHeatPlate(heatPl);
      // Run the simulation
      simulate(heatPl);
      // Add the plate to the vector
      plates.push_back(heatPl);
    }  // end if
  }  // end while
  // the master process will send a -1 to all processes to stop waiting
}

void Distribution::sendResults(
  std::vector<heatPlate*>& plates, int processNumber) {
  // Wait for the master process to request the results
  int processRequestNumber = 0;
  MPI_Recv(&processRequestNumber, 1, MPI_INT, 0, 12, MPI_COMM_WORLD,
           MPI_STATUS_IGNORE);
  if (processRequestNumber == processNumber) {
      // Send the number of plates
    int64_t plateCount = plates.size();
    MPI_Send(&plateCount, 1, MPI_INT64_T, 0, 13, MPI_COMM_WORLD);
    // Send the results
    for (int plater = 0; plater < plateCount; plater++) {
      // send the heatPlateNumber first
      if (MPI_Send(&plates[plater]->heatPlateNumber, 1, MPI_INT64_T, 0, 14,
        MPI_COMM_WORLD) == MPI_SUCCESS) {
        MPI_Send(&plates[plater]->generation, 1, MPI_INT64_T, 0, 14,
                MPI_COMM_WORLD);
      }  // end if
    }  // end for
  }  // end if
}  // end procedure

void Distribution::receiveResults(std::vector<heatPlate*>& plates,
                                  int processCount) {
  for (int process = 1; process < processCount; process++) {
    // tell processes to send their results
    MPI_Send(&process, 1, MPI_INT, process, 12, MPI_COMM_WORLD);
    int plateCount = 0;
    // receive the number of plates
    MPI_Recv(&plateCount, 1, MPI_INT64_T, process, 13, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    // receive the plates
    for (int plater = 0; plater < plateCount; plater++) {
      // receive the heatPlateNumber first
      int64_t id = 0;
      if (MPI_Recv(&id, 1, MPI_INT64_T, process, 14,
        MPI_COMM_WORLD, MPI_STATUS_IGNORE) == MPI_SUCCESS) {
           MPI_Recv(&plates[id]->generation, 1, MPI_INT64_T, process, 14,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          plates[id]->timeElapsed = plates[id]->generation * plates[id]->deltaT;
      }  // end if
    }  // end for
  }  // end for
}  // end for


void Distribution::runDistSimulation(std::string jobFileName, int processNumber,
                                     int processCount) {
  // get all plates in the job file
  std::vector<heatPlate*> heatPlates;
  if (processNumber == 0) {
    std::vector<std::vector<std::string>> parametersVector;
    std::vector<std::string> plates = FileManager::getPlates(jobFileName);
    Log::Report report(jobFileName);
    std::string path = jobFileName.substr(0, jobFileName.find_last_of("/") + 1);
    // For each plate in the job file
    for (uint64_t plater = 0; plater < plates.size(); plater++) {
      // Create a heat plate struct
      std::string plateInfo = plates[plater];
      //  tests/job001/job001.txt
      //  tests/job001/ + plate001.bin
      std::vector<std::string> parameters =
                                FileManager::getSimulationParameters(plateInfo);
      heatPlate* heatPl = new heatPlate(path, parameters, plater);
      heatPlates.push_back(heatPl);
      parametersVector.push_back(parameters);
    }  // end for
    distributeHeatPlates(heatPlates, processCount);
    receiveResults(heatPlates, processCount);
    int count = 0;
    for (auto parameters : parametersVector) {
      report.writeRow(parameters);
      report.writeResult(heatPlates[count]->generation,
                         heatPlates[count]->timeElapsed);
      count++;
    }  // end for
    report.writeReport();
  } else {
    simulateHeatPlates(heatPlates, processNumber);
    sendResults(heatPlates, processNumber);
    for (size_t writer = 0; writer < heatPlates.size(); writer++) {
      heatPlate* heatPl = heatPlates[writer];
      std::string outputPath = heatPl->plateName + "-" +
                               std::to_string(heatPl->generation);
      Log::binaryMatrixWriter(outputPath, heatPl->currentMatrix);
    }  // end for
  }  // end if
}  // end procedure

void Distribution::startDistribution(int argc, char * argv[]) {
  // initialize the MPI environment
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    int processNumber = -1;  // processNumber
    MPI_Comm_rank(MPI_COMM_WORLD, &processNumber);
    //  Get the number of nodes devices
    int processCount = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);
    try {
      std::string jobFileName = argv[1];
      int threadCount = omp_get_max_threads();
      if (argc == 3) {  // If thread count is specified
        try {
          if (std::atoi(argv[2]) > 0) threadCount = std::atoi(argv[2]);
        } catch(std::exception& e) {
          std::cerr << "Error: Incorrect parameter (2)" << std::endl;
        }  // end try/catch
      }  // end if
      omp_set_num_threads(threadCount);
      if (processCount == 1) {  // Run it alone
        runSimulation(jobFileName);
      } else {  // Run it by distributing it
        runDistSimulation(jobFileName, processNumber, processCount);
      }  // end if
      MPI_Finalize();
    } catch(std::exception& e) {
      std::cerr << "Error: Incorrect parameter (1)" << std::endl;
    }  // end try/catch
  }  // end if
}  // end procedure
