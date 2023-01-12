// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0
// Serial web server's initial code for parallelization

#ifdef WEBSERVER

#include "HttpServer.hpp"
#include "GoldbachWebApp.hpp"

/// Start the web server
int main(int argc, char* argv[]) {
  // Create the web server
  // I can have any number of web apps in the same server
  // Register the web application(s) with the web server
  HttpServer::getInstance().chainWebApp(&GoldbachWebApp::getInstance());
  // Start the web server
  return HttpServer::getInstance().start(argc, argv);
  /*
  command line arguments:
  start: start the server
  stop: stop the server
  restart: restart the server
  status: show the server status
  test: test the server
  config: config from file
  */
  // A server is a piece of software that listens to for requests, they sleep
  // until a request arrives, then they wake up and process the request
  // from the hardware perspective, a server is a speacialized computer
  // that is always on, and it is connected to the internet. It usually has
  // more RAM, storage and CPU cores than a regular computer.
}

#endif  // WEBSERVER
