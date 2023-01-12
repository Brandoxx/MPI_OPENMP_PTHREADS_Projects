// @copyright c_la_vie 2022
#include "fileManager.hpp"
#include "HeatDistSimulation.hpp"

void FileManager::readMatrix(heatPlate* heatPl,
                             std::string filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Error opening file " << filename << std::endl;
    throw std::runtime_error("Error opening file");
  }  // end if
  // read cols and rows
  size_t rows, cols;
  file.read((char*)&rows, 8); // NOLINT
  file.read((char*)&cols, 8);  // NOLINT
  // set the matrix atributes to the plate
  heatPl->setHeatPlateAtributes(rows, cols);
  for (int64_t row = 0; row < heatPl->rows; ++row) {
    for (int64_t col = 0; col < heatPl->cols; ++col) {
      double number = 0.0;
      file.read((char*)&number, 8); // NOLINT
      heatPl->matrix[row][col] = number;
    }  // end for
  }  // end for
  file.close();
}  // end procedure

std::vector<std::string> FileManager::getPlates(std::string filename) {
  // open the file
  std::ifstream file(filename);
  // check if the file is open
  if ( !file.is_open() ) {
    std::cerr << "Error opening file " << filename << std::endl;
    throw std::runtime_error("Error opening file");
  }
  // create a vector of strings to store the lines
  std::vector<std::string> plates;
  // read the file line by line in a string buffer called line
  std::string line;
  // while there is a line to read
  while (std::getline(file, line)) {
    // read the line and store it in the vector
    plates.push_back(line);
  }
  // close the file
  file.close();
  return plates;
}

std::vector<std::string> FileManager::getSimulationParameters(
                                      std::string plateInfo) {
  // get the size of the plateInfo string
  int size = plateInfo.size();
  // create a vector of strings to store the lines
  std::vector<std::string> parameters;
  // create a string buffer to store the parameter
  std::string temp = "";
  // define the delimiter
  char separator = ' ';
  // iterate over the string
  for (int i = 0; i < size; ++i) {
    // if the character is not a space
    if (plateInfo[i] != separator) {
      // add the character to the string buffer
      temp += plateInfo[i];
    } else {
      // if the character is a space
      if (!temp.empty()) {
        // add the string buffer to the vector
        parameters.push_back(temp);
      }
      // clear the string buffer
      temp.clear();
    }
  }
  // add the last parameter to the vector
  if (!temp.empty()) {
    parameters.push_back(temp);
  }
  // return the vector
  return parameters;
}

std::string FileManager::cleanExtension(std::string& filename,
                                        std::string extension) {
  // get the size of the filename
  // int size = filename.size();
  // get the size of the extension
  int extSize = extension.size();
  // get the position of the extension
  size_t pos = filename.find(extension);
  // if the extension is found
  if (pos != std::string::npos) {
    // remove the extension
    filename.erase(pos, extSize);
  }  // end if
  return filename;
}
