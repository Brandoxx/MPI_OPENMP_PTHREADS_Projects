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

#ifndef GOLDBACHSOLVERS_HPP
#define GOLDBACHSOLVERS_HPP

#include <vector>
#include <string>
#include "prodcons/Assembler.hpp"
#include "common/common.hpp"
#include "common/Log.hpp"
#include "GoldbachCalculator.hpp"

/**
* @brief GoldbachSolvers calculates all series of Goldbach
* @details This class consumes incomplete gb_data_t struct
* Resolves all goldbach series and sends complete struct now
*/
class GoldbachSolvers : public Assembler<gb_data_t*, gb_data_t*>{
  /*! \cond PRIVATE */
  /// Disable default methods for copying objects.
  DISABLE_COPY(GoldbachSolvers);
  /*! \endcond */

 protected:
  // Stop Condition GoldbachSolvers get by default
  // gb_data_t stopCondition = gb_data_t{nullptr, 0 ,0 ,0 ,0 ,{0}} ;
  // Goldbach class for calcutations
  gb_data_t* stopCondition;

 public:
 /**
  * @brief Construct a new Goldbach Solvers object
  * 
  * @param inputQueue the queue gb_data_t to solve
  * @param outputQueue the queue of gb_data_t structs
  */
  GoldbachSolvers(Queue<gb_data_t*>* inputQueue, Queue<gb_data_t*>* outputQueue)
                  : Assembler(inputQueue, outputQueue) {
    this->stopCondition = nullptr;
  }
  /**
   * @brief Destroy the Goldbach Solvers object
   */
  ~GoldbachSolvers() override {}
  /**
   * @brief override the run method herited from Assembler and from Thread.
   * @return int with error code.
   */
  int run() override;
  /**
   * @brief override the stop method herited from Assembler and from Thread.
   * @return int with error code.
   */
  void consume(gb_data_t* inputData) override;
  /**
   * @brief override consume forever method herited from consumer and Thread.
   */
  void consumeForever() override;
};

#endif
