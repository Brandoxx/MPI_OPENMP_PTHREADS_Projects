// @copyright C_la_vie 2022
#include "GoldbachPacker.hpp"

bool GoldbachPacker::isComplete(std::vector<gb_data_t*> numbers,
                                gb_data_t* data) {
    // If last element of queue is the same as last element of list then queue
    // complete
    if (static_cast<int>(numbers.size()) == data->packetTotal) {
        // Merge queue
        return true;
    }
    return false;
}

void GoldbachPacker::consume(gb_data_t* data) {
    if (data->packetTotal < 0) {
        // Sent it directly to HttpDispatcher
        std::vector<gb_data_t*> vec;
        vec.push_back(data);
        this->produce(vec);
    } else {
        // Consumes gb_data_t
        clientRequest* tempClient = data->requestID;
        std::vector<gb_data_t*>* myVector = this->getVector(tempClient);
        myVector->push_back(data);
        bool state = this->isComplete(*myVector, data);
        if (state) {
          this->produce(*myVector);
          delete myVector;
          vectorMap.erase(tempClient);
        }
    }
}

std::vector<gb_data_t*>* GoldbachPacker::getVector(clientRequest* user) {
  // Finds the queue that belongs to the user given
  auto result = this->vectorMap.find(user);
  if (result == this->vectorMap.end()) {
    std::vector<gb_data_t*>* newQueue = new std::vector<gb_data_t*>();
    vectorMap[user] = newQueue;
    return newQueue;
  }
  return result->second;
}

/**
 * @brief Calls consume forever
 * @return int 
 */
int GoldbachPacker::run() {
    //  The amount of solver is equal to threads available in hardware
    //  See GoldbachWebApp::start(), GoldbachWebApp.cpp
    size_t solverCount = std::thread::hardware_concurrency();
    //  Each time one solver sends stcondition
    // GoldbachPacker will execute consume forever until
    // all solvers have sent stop condition
    for (size_t index = 0; index < solverCount; ++index) {
        this->consumeForever();
    }
    this->getProducingQueue()->push(std::vector<gb_data_t*>());
    return 0;
}
