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

#ifndef GOLDBACHCALCULATOR_HPP
#define GOLDBACHCALCULATOR_HPP

#include <cstdlib>
#include <vector>
#include "common/UtilStruct.hpp"

/** @class Goldbach
 * @brief A class that calculates a series of Goldbach of a list of given numbers
 */
class Goldbach {
 private:
    /**
    * @brief Get all prime numbers up to a specific input
    * @param number of reference
    * @return std::vector<int> with all prime numbers
    */
    std::vector<int> getPrimeNumbers(int number);

    /**
    * @brief Goldbach Conjecture's method for even numbers
    * @param number Integer whose Goldbach series are to be calculated
    * @param primes Vector with all primes for work
    * @return gb_data_t with all information about Goldbach process
    */
    gb_data_t* evenGoldbach(int number, std::vector<int> primes);

    /**
    * @brief Boolean if number is found in vector
    * @param number Integer to be found in the array
    * @param vector Collection of numbers in which it will be found
    * @return Boolean, true if find input number, else false
    */
    bool findNumber(int number, std::vector<int> vector);

    /**
    * @brief Goldbach Conjecture's method for odd numbers
    * @param number Integer whose Goldbach series are to be calculated
    * @param primes Vector with all primes for work
    * @return gb_data_t with all information about Goldbach process
    */
    gb_data_t* oddGoldbach(int number, std::vector<int> primes);

 public:
    /**
    * @brief Get the instance object of Goldbach calculator
    * @return Goldbach& as a single instance of an application
    */
    static Goldbach& getInstance() {
      static Goldbach instance;
      return instance;
    }
    /**
    * @brief Found all Goldbach series from vector of integers
    * @param input gb_data_t to be processed
    * @return gb_data_t* with all information of goldbach series found
    * found from the number given
    * @note This method also accepts negative numbers
    */
    gb_data_t* goldbachUnitSolver(gb_data_t* input);
};  //  class Goldbach

#endif
