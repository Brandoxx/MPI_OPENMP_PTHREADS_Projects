// @copyright C_la_vie 2022

#include "GoldbachSolvers.hpp"

int GoldbachSolvers::run() {
  this->consumeForever();
  return 0;
}

void GoldbachSolvers::consume(gb_data_t* inputData) {
  // Consumes gb_data_t's and produces with solvers until it encounters it's
  // stop condition
  if (inputData == nullptr) {
    produce(this->stopCondition);
  } else {
    gb_data_t* product = Goldbach::getInstance().goldbachUnitSolver(inputData);
    produce(product);
  }
}

void GoldbachSolvers::consumeForever() {
  assert(this->consumingQueue);
  while (true) {
    // Get the next data to consume, or block while queue is empty
    gb_data_t* data = this->consumingQueue->pop();
    // If data is the stop condition, stop the loop
    if (data == this->stopCondition) {
      consume(nullptr);
      break;
    }
    // Process this data
    // this is the method that will be implemented by the child class
    this->consume(data);
  }
}
