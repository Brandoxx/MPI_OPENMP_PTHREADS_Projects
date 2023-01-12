// @copyright C_la_vie 2022

#include <iostream>

#include "GoldbachCalculator.hpp"

std::vector<int> Goldbach::getPrimeNumbers(int number) {
    std::vector<int> output;
    int* allnumbers = reinterpret_cast<int*>(calloc((number), sizeof(int)));
    for (int counter = 0; counter < number; counter++) {
        allnumbers[counter] = counter+2;
    }
    for (int counter = 0; counter < (number - 1); counter++) {
        int actualNumber = allnumbers[counter];
        if (actualNumber != 0) {
            output.push_back(actualNumber);
            for (int equalizer = counter; equalizer < (number - 1);
                    equalizer += actualNumber) {
                allnumbers[equalizer] = 0;
            }
        }
    }
    free(allnumbers);
    return output;
}

Goldbach::gb_data_t Goldbach::evenGoldbach(int number,
                                           std::vector<int> primes) {
    Goldbach::gb_data_t output;
    std::vector<int64_t> sums;
    int biggerCounter = primes.size() - 1;
    int bigger = primes.at(biggerCounter);
    int minorCounter = 0;
    int minor = primes.at(minorCounter);
    while (minor <= bigger) {
        if (minor + bigger == number) {
            sums.push_back(minor);
            sums.push_back(bigger);
            try {
                biggerCounter--;
                bigger = primes.at(biggerCounter);
            } catch (const std::out_of_range& oor) {
                minorCounter++;
                minor = primes.at(minorCounter);
            }
        } else {
            if (minor + bigger > number) {
                try {
                    biggerCounter--;
                    bigger = primes.at(biggerCounter);
                }
                catch (const std::out_of_range& oor) {
                    minorCounter++;
                    minor = primes.at(minorCounter);
                }

            } else {
                try {
                    minorCounter++;
                    minor = primes.at(minorCounter);
                } catch (const std::out_of_range& oor) {
                    biggerCounter--;
                    bigger = primes.at(biggerCounter);
                }
            }
        }
    }
    output.totalAddends = (sums.size()) / 2;
    output.goldbachAddends = sums;
    return output;
}

Goldbach::gb_data_t Goldbach::oddGoldbach(int number, std::vector<int> primes) {
    Goldbach::gb_data_t output;
    std::vector<int64_t> sums;
    for (unsigned int counter = 0; counter < primes.size(); counter++) {
        int minor = primes.at(counter);
        int mediumCounter = counter;
        int medium = primes.at(mediumCounter);
        int biggerCounter = primes.size() - 1;
        int bigger = primes.at(biggerCounter);
        while (medium <= bigger) {
            if (minor + medium + bigger == number) {
                sums.push_back(minor);
                sums.push_back(medium);
                sums.push_back(bigger);
                biggerCounter--;
                bigger = primes.at(biggerCounter);
            } else {
                if (minor + medium + bigger > number) {
                    biggerCounter--;
                    bigger = primes.at(biggerCounter);
                } else {
                    mediumCounter++;
                    medium = primes.at(mediumCounter);
                }
            }
        }
    }
    //  Give format to output package
    output.totalAddends = (sums.size()) / 3;
    output.goldbachAddends = sums;
    return output;
}

std::vector<Goldbach::gb_data_t> Goldbach::goldbachMaker(
    std::vector<int64_t> inputStream) {
    //  Get prime numbers
    int bigger = 0;
    for (unsigned int counter = 0; counter < inputStream.size(); counter++) {
        int number = inputStream.at(counter);
        if (number < 0) {
            number *= -1;
        }
        if (number > bigger) {
            bigger = number;
        }
    }
    std::vector<int> primeNumbers = Goldbach::getPrimeNumbers(bigger);
    Goldbach::gb_data_t package;
    std::vector<Goldbach::gb_data_t> output;
    int number;
    for (unsigned counter = 0; counter < inputStream.size(); counter++) {
        number = inputStream.at(counter);
        if (number < 0) {
            number *= -1;
        }
        //  Filters for no valid inputs
        if (number <= 3 || number == 5) {
            //  Give format to package of invalid input
            package.number = inputStream.at(counter);
            package.format = 0;
            package.totalAddends = 0;
            package.goldbachAddends = std::vector<int64_t>{0};
        } else {
            if (number % 2 == 0) {
                package = Goldbach::evenGoldbach(number, primeNumbers);
                package.number = inputStream.at(counter);
                package.format = 2;
            } else {
                package = Goldbach::oddGoldbach(number, primeNumbers);
                package.number = inputStream.at(counter);
                package.format = 3;
            }
        }
        output.push_back(package);
    }
    return output;
}
