#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <cassert>

const char* help = "Usage: binaryComparison <file1> <file2> <epsilon>";

/**
 * @brief Reads a binary file and returns a matrix with the values inside the
 * file. This asumes that the file has the following format:
 * - 8 bytes with the number of rows
 * - 8 bytes with the number of cols
 * - 8 bytes for each value in the matrix
 * @param filename 
 * @return std::vector<std::vector<double>> 
 */
std::vector<std::vector<double>> readMatrix(std::string filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Error opening file " << filename << std::endl;
    throw std::runtime_error("Error opening file");
  }  // end if
  // read cols and rows
  size_t rows, cols;
  file.read((char*)&rows, 8);  // NOLINT
  file.read((char*)&cols, 8);  // NOLINT
  // set the matrix atributes to the plate
  std::vector<std::vector<double>> matrix(rows, std::vector<double>(cols));
  for (size_t row = 0; row < rows; ++row) {
    for (size_t col = 0; col < cols; ++col) {
      double number = 0.0;
      file.read((char*)&number, 8); // NOLINT
      matrix[row][col] = number;
    }  // end for
  }  // end for
  file.close();
  return matrix;
}  // end procedure

/**
 * @brief Checks if the values inside the matrices are equal with the given
 * epsilon
 * 
 * @param firstMatrix First matrix to compare
 * @param secondMatrix Second matrix to compare
 * @param epsilon Epsilon value to compare the values, maximum difference
 * @return true if the values are equal 
 * @return false if the values are not equal
 */
bool compareTemperaturePlates(std::vector<std::vector<double>> firstMatrix,
 std::vector<std::vector<double>> secondMatrix, double epsilon) {
  bool equal = true;
  for (size_t row = 0; row < firstMatrix.size(); ++row) {
    for (size_t col = 0; col < firstMatrix[0].size(); ++col) {
      double firstValue = firstMatrix[row][col];
      double secondValue = secondMatrix[row][col];
      double diff = fabs(firstValue - secondValue);
      if (diff > epsilon) {
        equal = false;
        break;
      }  // end if
    }  // end for
  }  // end for
  return equal;
}  // end procedure

/**
 * @brief Compares two matrices sizes and calls the function to compare the
 * values
 *  
 * @param firstMatrix First matrix to compare
 * @param secondMatrix Second matrix to compare
 * @param epsilon Epsilon value to compare the values, maximum difference
 *  between the values to be considered equal 
 * @return true if the matrices are equal
 * @return false if the matrices are not equal
 */
bool compareMatrices(std::vector<std::vector<double>> firstMatrix,
 std::vector<std::vector<double>> secondMatrix, double epsilon) {
  bool equal = firstMatrix.size() == secondMatrix.size();  // check if the
  // matrices have the same number of rows
  if (equal) {
    equal = firstMatrix[0].size() == secondMatrix[0].size();  // check if the
    // matrices have the same number of cols
    if (equal) {
      // check if the values are equal in the matrices with the given epsilon
      equal = compareTemperaturePlates(firstMatrix, secondMatrix, epsilon);
    }
  }
  return equal;
}

int main(int argc, char** argv) {
  if (argc != 4) {
      std::cout << help << std::endl;
      return 1;
  }  // end if
  std::string firstPath = argv[1];  // get the first file path
  std::string secondPath = argv[2];  // get the second file path
  double epsilon = std::stod(argv[3]);  // get the epsilon value
  std::vector<std::vector<double>> firstMatrix = readMatrix(firstPath);  // read
  // the first matrix
  std::vector<std::vector<double>> secondMatrix = readMatrix(secondPath);
  // read the second matrix
  bool equal = compareMatrices(firstMatrix, secondMatrix, epsilon);  // compare
  // the matrixes
  if (equal) {
    std::cout << "The matrices are equal" << std::endl;
  } else {
    std::cout << "The matrices are not equal" << std::endl;
  }
  return 0;
}