// @copyright C_la_vie 2022
#include "HttpRequestHandler.hpp"

// For general purpose instead of just for the only app we have.
int HttpRequestHandler::extractKey(const clientRequest* data) {
  // Finds URI according to the app given and handles certain specific errors
  if (data->request->getURI().rfind("/favicon.ico", 0) != std::string::npos) {
    data->request->getSocket().close();
    // Frees request that will not be used
    delete data->request;
    delete data->response;
    delete data;
    return 0;
  } else {
    if (data->request->getURI().rfind("/", 0) != std::string::npos) {
      return 1;
    } else {
      // Frees request that will not be used
      data->request->getSocket().close();
      delete data->request;
      delete data->response;
      delete data;
    }
  }
  return 0;
}

void HttpRequestHandler::consume(clientRequest* data) {
  // Finds key given on the URI
  const int& key = this->extractKey(data);
  if (key == 0) {
    return;
  }
  const auto& itr = this->toQueues.find(key);
  if (itr == this->toQueues.end()) {
    throw std::runtime_error("dispatcher: queue's key not found");
  }
  itr->second->push(data);
}

void HttpRequestHandler::consumeForever() {
  assert(this->consumingQueue);
  while (true) {
    //  Get the next data to consume, or block while queue is empty
    clientRequest* data = this->consumingQueue->pop();
    //  If data is the stop condition, stop the loop
    if ( data == this->stopCondition ) {
      // If we recieve the stop condition, we propagate it to the other
      // threads.
      for (auto& queue : this->toQueues) {
          queue.second->push(nullptr);
      }
      break;
    }
    //  Process this data
    //  this is the method that will be implemented by the child class
    this->consume(data);
  }
}

int HttpRequestHandler::run() {
  this->consumeForever();
  return 0;
}
