// @copyright c_la_vie 2022
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <exception>

struct heatPlate;

namespace FileManager {
  /// @brief reads a matrix from a file
  /// @param heatPl is the heat plate that will hold the matrix
  /// @param filename is the name of the file that contains the matrix
  /// @throws std::runtime_error if the file cannot be opened
  void readMatrix(heatPlate* heatPl, std::string filename);
  /// @brief extracts each line from the input file and puts it in a vector of
  /// strings
  /// @param filename is the name of the file that contains the plates
  /// @return a vector of strings that contains each line of the input file
  /// @throws std::runtime_error if the file cannot be opened
  std::vector<std::string> getPlates(std::string filename);
  /// @brief fills with the parameters of the simulation a vector of strings
  /// @param plateInfo is a string containing the parameters of the simulation
  /// @return a vector of strings that contains the parameters of the simulation
  std::vector<std::string> getSimulationParameters(std::string plateInfo);
  /// @brief erases the extension characters of a file name
  /// @param filename is the name of the file
  /// @param extension used by the file given
  /// @return a string that contains the name of the file without the extension
  std::string cleanExtension(std::string& filename, std::string extension);
}  // namespace FileManager
