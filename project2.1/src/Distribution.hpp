// @copyright c_la_vie 2022
#ifndef DISTRIBUTION_HPP
#define DISTRIBUTION_HPP
#include <mpi.h>
#include <vector>
#include <string>
#include <array>

struct heatPlate;

namespace Distribution {
  /**
   * @brief Sends a heat plate to a process
   * @param heatPl the heat plate to be sent
   * @param dest the destination process
   */
  void sendHeatPlate(heatPlate* heatPl, int dest);

  /**
   * @brief Receives a heat plate from a process
   * @param heatPl the heat plate to be received
   */
  void receiveHeatPlate(heatPlate* heatPl);

  /**
   * @brief Distributes the plates in a jobfile between processes.
   * @details it must be executed by the master process (process 0)
   * @param plates the plates in the jobfile
   * @param processCount the number of processes
   */
  void distributeHeatPlates(std::vector<heatPlate*>& plates, int processCount);

  /**
   * @brief Requests a heat plate from a process 0 and receives it.
   * @details must be executed by a process other than 0
   * @param plates the vector of plates to be filled
   * @param processNumber the number of the process
   */
  void simulateHeatPlates(std::vector<heatPlate*>& plates, int processNumber);

  /**
   * @brief Sends the results of a simulation to the master process
   * @param plates 
   * @param processNumber 
   */
  void sendResults(std::vector<heatPlate*>& plates, int processNumber);
  /**
   * @brief Receives the results of a simulation from a process
   * @param plates 
   * @param processCount 
   */
  void receiveResults(std::vector<heatPlate*>& plates, int processCount);

  /**
   * @brief Runs the distributed simulation either alone or distributed, depending on how many processes are selected
   * @param jobFileName is the file that is to be read
   * @param processCount amount of total processes
   * @param processNumber is the actual process working
   */
  void runDistSimulation(std::string jobFileName, int processCount,
                         int processNumber);

  /**
   * @brief Starts the mpi environment
   * @param argc is the number of arguments
   * @param argv are the arguments
   */
  void startDistribution(int argc, char * argv[]);
}  // namespace Distribution
#endif  // DISTRIBUTION_HPP
