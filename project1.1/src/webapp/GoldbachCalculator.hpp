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

#include <cstdlib>
#include <vector>

/** @class Goldbach
 * @brief A class that calculates a series of Goldbach of a list of given numbers
 */
class Goldbach {
 public:
   /** @struct gb_data_t
   *  @brief This structure stores data required to calculate goldbachAddends
   *  @var: gb_data_t::number
   *  Member number contains of reference to get goldbach series
   *  @var: gb_data_t::format
   *  Member format contains a reference expressing how to read goldbach series
   *  @var: gb_data_t::totalAddends
   *  Member totalAddends contains the total number of series found
   *  @var: gb_data_t::goldbachAddends
   *  Member goldbachAddends contains all the goldbach series found
   */
     struct gb_data_t {
        int number;
        int format;
        int totalAddends;
        std::vector<int64_t> goldbachAddends;
     };

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
    gb_data_t evenGoldbach(int number, std::vector<int> primes);

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
    gb_data_t oddGoldbach(int number, std::vector<int> primes);

 public:
    /**
    * @brief Found all Goldbach series from vector of integers
    * @param inputStream Vector with all input integers
    * @return vector<gb_data_t> with all information of goldbach series found
    * found from the input stream
    * @note This method also accepts negative numbers
    */
    std::vector<Goldbach::gb_data_t> goldbachMaker(std::vector<int64_t>
                                                   inputStream);
};  //  class Goldbach
