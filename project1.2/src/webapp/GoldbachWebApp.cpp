// @copyright C_la_vie 2022

#include <algorithm>
#include <cassert>
#include <iostream>
#include <regex>
#include <stdexcept>
#include "GoldbachWebApp.hpp"

/**
 * @brief return solverOutputQueue
 * @return solverOutputQueue
*/
Queue<gb_data_t*>* GoldbachWebApp::getSolversOutputQueue() {
  return solverOutputQueue;
}

int GoldbachWebApp::run() {
  // Starts running the app
  this->start();
  return 0;
}

void GoldbachWebApp::start() {
  //  Create all assemblers and queues of assembly line
  this->packer = new GoldbachPacker();
  this->dispatcher = new HttpDispatcher();
  this->solverOutputQueue = new Queue<gb_data_t*>();
  this->packerOutputQueue = new Queue<std::vector<gb_data_t*>>();
  this->setOwnProducingQueue();
  //  Start GoldbachSolvers
  this->solverCount = std::thread::hardware_concurrency();
  //  Create and inicialize each solver with a single instance of Goldbach
  for (size_t index = 0; index < this->solverCount; ++index) {
    GoldbachSolvers* solver = new GoldbachSolvers(solverInputQueue,
                                                  solverOutputQueue);
    solver->startThread();
    this->solvers.push_back(solver);
  }
  // Set packer properties
  this->packer->setConsumingQueue(solverOutputQueue);
  this->packer->setProducingQueue(packerOutputQueue);
  this->packer->startThread();
  // Set dispatcher properties
  this->dispatcher->setConsumingQueue(packerOutputQueue);
  this->dispatcher->startThread();
  this->consumeForever();
  stop();
}

/**
 * @brief create solverInputQueue of this object
*/
void GoldbachWebApp::setOwnProducingQueue() {
  this->solverInputQueue = new Queue<gb_data_t*>();
}

void GoldbachWebApp::stop() {
  // When stop condition is received, send stop condition to solvers
  for (size_t index = 0; index < this->solverCount; ++index) {
    this->solverInputQueue->push(nullptr);
  }
  // Wait until solvers stop
  for (size_t index = 0; index < this->solverCount; ++index) {
    solvers[index]->waitToFinish();
    delete solvers[index];
  }
  // Destroy inputQueue to solvers
  // Destroy outputQueue
  this->packer->waitToFinish();
  this->dispatcher->waitToFinish();
  // Free dinamic memory used
  delete this->solverOutputQueue;
  delete this->packerOutputQueue;
  delete this->solverInputQueue;
  delete this->packer;
  delete this->dispatcher;
}

void GoldbachWebApp::consume(clientRequest* request) {
  handleClientRequest(request);
}

void GoldbachWebApp::consumeForever() {
  //  Always Consume
  while (true) {
    clientRequest* request = this->consumingQueue->pop();
    if (request == this->stopCondition) {
      break;
    } else {
      this->handleClientRequest(request);
    }
  }
}

bool GoldbachWebApp::handleClientRequest(clientRequest *request) {
  HttpResponse httpResponse = *request->response;
  HttpRequest httpRequest = *request->request;
  // If the home page was asked
  if (httpRequest.getMethod() == "GET" && httpRequest.getURI() == "/") {
    return this->serveHomePage(request);
  }
  // If the request starts with "Goldbach/" is for this web app
  if (httpRequest.getURI().rfind("/Goldbach", 0) == 0) {
    // serve goldbach here!
    return this->serveGoldbach(request);
  }
  // Unrecognized request
  return false;
}

std::pair<int, std::vector<int64_t>> GoldbachWebApp::analyzeURI(
                                                              std::string URI) {
  std::pair<int, std::vector<int64_t>> reaction = {0, std::vector<int64_t>()};
  // If webapp should response to this request
  for (size_t index = 0; index < URI.length(); index++) {
    if (URI[index] == ',') {
      URI.replace(index, 1, "%2C");
    }
  }
  //  Extract numbers
  std::smatch matches;
  //  Find matches
  std::regex inQuery("^/Goldbach(/|\\?number=)([+-]?\\d+(%2C[+-]?\\d+)*)$");
  if (std::regex_search(URI, matches, inQuery)) {
    assert(matches.length() >= 3);
    std::string numberString = matches[2];
    size_t numberEndIndex = 0;
    std::string tempString = "";
    //  Found all numbers in input stream
    while ((numberEndIndex = numberString.find("%2C")) != std::string::npos) {
      tempString = numberString.substr(0, numberEndIndex);
      try {
        // catch all number with a comma before
        (reaction.second).push_back(std::stoll(numberString));
      } catch(std::out_of_range const& ex) {}
      numberString.erase(0, numberEndIndex + 3);
    }
    try {
      // to catch the last number because it doesn't have a comma
      (reaction.second).push_back(std::stoll(numberString));
    } catch(std::out_of_range const& ex) {}
    //  Flags
    if ((reaction.second).empty()) {
      reaction.first = -2;  // Server out of bounds
    } else {
      reaction.first = 0;  // Valid Request
    }
  } else {
    reaction.first = -1;  // Invalid request
  }
  return reaction;
}

bool GoldbachWebApp::serveGoldbach(clientRequest* request) {
  HttpResponse httpResponse = *request->response;
  HttpRequest httpRequest = *request->request;
  (void)httpRequest;
  // Set HTTP response metadata (headers)
  httpResponse.setHeader("Server", "AttoServer v1.0");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");
  // If a number was asked in the form "/Goldbach/1223"
  // or "/Goldbach?number=1223"
  std::string URI = httpRequest.getURI();
  // Send the response to the client (user agent)
  bool response = false;
  std::pair<int, std::vector<int64_t>> reaction = this->analyzeURI(URI);

  //  In case of valid request
  if (reaction.first == 0) {
    //  For each gb_data_t push it in queue
    for (size_t index = 0; index < reaction.second.size(); index++) {
      int64_t number = reaction.second[index];
      gb_data_t* data = new gb_data_t();
      data->requestID = request;
      data->number = number;
      data->packetTotal = reaction.second.size();
      this->solverInputQueue->push(data);
    }
  } else if (reaction.first == -1) {  // if the request is invalid
      gb_data_t* data = new gb_data_t();
      data->requestID = request;
      data->number = 1;  // default number for the solver so that
      // the know that the request is invalid
      data->packetTotal = -1;  // we use a negative number in packetTotal to
      // as a flag for invalid request
      this->solverInputQueue->push(data);
  } else if (reaction.first == -2) {  // if the request is out of bounds
      gb_data_t* data = new gb_data_t();
      data->requestID = request;
      data->number = 1;
      data->packetTotal = -2;
      this->solverInputQueue->push(data);
  }
  return response;
}

bool GoldbachWebApp::serveHomePage(clientRequest* request) {
  //  Create special package for serve Homepage
  gb_data_t* data = new gb_data_t();
  data->requestID = request;
  data->number = 1;
  data->packetTotal = -3;
  this->solverInputQueue->push(data);
  return 0;
}

