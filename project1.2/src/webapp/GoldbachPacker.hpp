/**
 * @author A. Badilla Olivas
 * @author B. Mora Umana
 * @author G. Molina Burgarelli
 * @author A. Madriz Aguero 
 * @version 0.1
 * @date 2022-10-11
 * 
 * @copyright 2022 GNU GENERAL PUBLIC LICENSE
 */

#ifndef GOLDBACHPACKER_HPP
#define GOLDBACHPACKER_HPP

#include <vector>
#include <string>
#include <map>

#include "prodcons/Assembler.hpp"
#include "common/common.hpp"
#include "common/Log.hpp"
#include "GoldbachCalculator.hpp"
/**
* @brief GoldbachSolvers calculates all series of Goldbach
* @details This class consumes incomplete gb_data_t struct
* Resolves all goldbach series and sends complete struct now
*/
class GoldbachPacker : public Assembler<gb_data_t*, std::vector<gb_data_t*>> {
 public:
 /**
  * @brief Construct a new Goldbach Packer object
  * 
  */
  GoldbachPacker() : Assembler<gb_data_t*, std::vector<gb_data_t*>>() {}
  /**
   * @brief Destroy the Goldbach Packer object
   */
  ~GoldbachPacker() override {}
  /**
   * @brief override the consume method herited from Assembler.
   * @return int with error code.
   */
  void consume(gb_data_t* data) override;
  /**
   * @brief override the run method herited from Assembler.
   * @return int with error code.
   */
  int run() override;

 private:
  /**
   * @brief Map of solutions vectors for each client request
   */
  std::map<clientRequest*, std::vector<gb_data_t*>*> vectorMap;
  /**
   * @brief get the vector of solutions for a client request
   */
  std::vector<gb_data_t*>* getVector(clientRequest* user);
  /**
   * @brief checks if a client request is already in the map and if it is
   * complete
   */
  bool isComplete(std::vector<gb_data_t*> numbers, gb_data_t* data);
};
#endif  // GOLDBACHPACKER_HPP
