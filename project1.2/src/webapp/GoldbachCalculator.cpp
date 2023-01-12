// @copyright C_la_vie 2022

#include <iostream>

#include "GoldbachCalculator.hpp"

std::vector<int> Goldbach::getPrimeNumbers(int number) {
    std::vector<int> output;
    int* allnumbers = new int[number]{};
    //  Create a list with all numbers in it to number
    for (int counter = 0; counter < number; counter++) {
        allnumbers[counter] = counter+2;
    }
    //  Iterates for each number of new list
    for (int counter = 0; counter < (number - 1); counter++) {
        int actualNumber = allnumbers[counter];
        if (actualNumber != 0) {
            output.push_back(actualNumber);
            //  Iterates actualNumber jump and set value in 0
            for (int equalizer = counter; equalizer < (number - 1);
                    equalizer += actualNumber) {
                allnumbers[equalizer] = 0;
            }
        }
    }
    delete[] allnumbers;
    return output;
}

gb_data_t* Goldbach::evenGoldbach(int number, std::vector<int> primes) {
    gb_data_t* output = new gb_data_t();
    std::vector<int64_t> sums;
    int biggerCounter = primes.size() - 1;
    int bigger = primes.at(biggerCounter);
    int minorCounter = 0;
    int minor = primes.at(minorCounter);
    //  While both parts dont crash each other
    while (minor <= bigger) {
        //  Goldbach sum found
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
            //  Goldbach sum not found
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
    //  Set properties of output
    output->totalAddends = (sums.size()) / 2;
    output->goldbachAddends = sums;
    return output;
}

gb_data_t* Goldbach::oddGoldbach(int number, std::vector<int> primes) {
    gb_data_t* output = new gb_data_t();
    std::vector<int64_t> sums;
    //  For each number of primes iterates
    for (unsigned int counter = 0; counter < primes.size(); counter++) {
        int minor = primes.at(counter);
        int mediumCounter = counter;
        int medium = primes.at(mediumCounter);
        int biggerCounter = primes.size() - 1;
        int bigger = primes.at(biggerCounter);
        //  While medium dont crash with bigger
        while (medium <= bigger) {
            //  Goldbach sum found
            if (minor + medium + bigger == number) {
                sums.push_back(minor);
                sums.push_back(medium);
                sums.push_back(bigger);
                biggerCounter--;
                bigger = primes.at(biggerCounter);
            } else {
                //  Goldbach sum not found
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
    output->totalAddends = (sums.size()) / 3;
    output->goldbachAddends = sums;
    return output;
}

gb_data_t* Goldbach::goldbachUnitSolver(gb_data_t* input) {
    gb_data_t* output;
    int absEntry = input->number;
    if (input->number < 0) {
        absEntry *= -1;
    }
    //  Filter no valid numbers
    if (absEntry <= 3 || absEntry == 5) {
        output = new gb_data_t();
        //  Give format to invalid input packages
        output->number = input->number;
        output->format = 0;
        output->totalAddends = 0;
        output->goldbachAddends = std::vector<int64_t>{0};
        output->packetTotal = input->packetTotal;
        output->requestID = input->requestID;
    //  Case of valid input number
    } else {
        //  Calculate prime numbers
        std::vector<int> primeNumbers = Goldbach::getPrimeNumbers(absEntry);
        //  Even case
        if (absEntry % 2 == 0) {
            output = Goldbach::evenGoldbach(absEntry, primeNumbers);
            output->format = 2;
        //  Odd case
        } else {
            output = Goldbach::oddGoldbach(absEntry, primeNumbers);
            output->format = 3;
        }
        output->number = input->number;
        output->packetTotal = input->packetTotal;
        output->requestID = input->requestID;
    }
    delete input;
    return output;
}
