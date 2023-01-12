// @copyright c_la_vie 2022
#ifndef LOG_HPP
#define LOG_HPP

#include <cassert>
#include <string>
#include <vector>
#include "fileManager.hpp"

namespace Log {  // Namespace Log
  static std::string formatTime(const time_t seconds) {
    char text[48];  // YYYY/MM/DD hh:mm:ss
    const std::tm& gmt = * std::gmtime(&seconds);
    snprintf(text, sizeof text, "%04d/%02d/%02d\t%02d:%02d:%02d", gmt.tm_year
        - 70, gmt.tm_mon, gmt.tm_mday - 1, gmt.tm_hour, gmt.tm_min, gmt.tm_sec);
    return text;
  }

  inline void binaryMatrixWriter(std::string filename,
                        std::vector<std::vector<double>> matrix) {
    // Opens output file
    std::string output = filename + ".bin";
    std::ofstream file;
    file.open(output, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file");
    }  // end if
    // Defines the data to be written and the length of it in the .bin
    size_t rows = matrix.size();
    size_t cols = matrix[0].size();
    file.write((char*)&rows, 8); // NOLINT
    file.write((char*)&cols, 8); // NOLINT
    // Writes it
    for (size_t row = 0; row < matrix.size(); row++) {
        for (size_t column = 0; column < matrix[row].size(); column++) {
            file.write(reinterpret_cast<char*>(&matrix[row][column]),
            sizeof(double));
        }  // end for
    }  // end for
    file.close();
  }  // end procedure

struct Report {
  // String in which the report will be filed
  std::string reportText;
  std::string path;
  explicit Report(std::string& path) {
    assert(path != "");
    this->path = path.erase(path.find_last_of("."));  // remove extension
  }

  /**
    * @brief Writes a report to a file
    * @throw std::runtime_error if the file cannot be opened
    */
  void writeReport() {
    std::ofstream reportFile;
    reportFile.open(this->path + ".tsv", std::ios::out);
    std::cout<< "report: " << this->reportText << std::endl;
    if (!reportFile.is_open()) {
      std::cerr << "Error opening file " << this->path << std::endl;
      throw std::runtime_error("Error opening file");
    } else {
      reportFile << this->reportText;
      reportFile.close();
    }
  }
  /**
  * @brief Adds a line with the specified parameters to the report
  */
  void writeRow(std::vector<std::string>& parameters) {
      for (auto parameter : parameters) {
          this->reportText += parameter + "\t";
      }
      this->reportText = this->reportText.erase(this->reportText.size() - 2, 1);
  }

  /**
  * @brief Adds how many generations it took and the time in which it happened
  */
  void writeResult(const int64_t& generation, const time_t& seconds) {
    std::string result = "";
    this->reportText.append(std::to_string(generation).data());
    this->reportText.append("\t");
    result = Log::formatTime(seconds);
    this->reportText.append(result + "\n");
  }
};
}  // namespace Log

#endif  // LOG_HPP
