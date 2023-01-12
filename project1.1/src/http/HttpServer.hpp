/**
 * @file HttpServer.hpp
 * @author A. Badilla Olivas
 * @author B. Mora Umana
 * @author G. Molina Burgarelli
 * @author A. Madriz Aguero 
 * @version 0.1
 * @date 2022-10-11
 * 
 * @copyright 2022 GNU GENERAL PUBLIC LICENSE
 */

#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <vector>

#include <csignal>

#include "network/TcpServer.hpp"
#include "http/HttpRequest.hpp"
#include "http/HttpResponse.hpp"
#include "http/HttpConnectionHandler.hpp"

#define DEFAULT_PORT "8080"  // default port

/**
 * @brief Handles signals
 * 
 * @param signum Signal to be handled
 */
void signalHandler(int signum);
/* 
the web is like a telephone numbers. We use numbers to identify chanels to 
comunicate with people. Imagine you want to call someone that is in a hotel.
You call the hotel (ip address) and ask for the room number of the person 
you want to talk (port). The hotel will connect you to the room and you can 
finally talk to the person. The hotel is the web server, the room is the port 
*/
class HttpApp;
/**
 * @brief this class is the web server.
 * @details it start a tcp server and listes for connections. When a connection
 * is made, it creates a puts the socket in a queue of sockets. A thread pool of
 * connection handlers will consume the sockets and handle the requests.
 * @param applications are the applications that will be used to handle requests
 * @param handlers are the threads that will handle the requests
 * @param socketQueue is the queue of sockets
 * @param connectionHandlers is the amount of threads that will handle sockets
 */
class HttpServer : public TcpServer {
  DISABLE_COPY(HttpServer);  // tcp means transfer control protocol

 protected:
  /// Lookup criteria for searching network information about this host
  struct addrinfo hints;
  /// TCP port where this web server will listen for connections
  const char* port = DEFAULT_PORT;
  /// Number of connection handlers
  uint64_t connectionHandlers = std::thread::hardware_concurrency();
  /// Vector of connection handlers
  std::vector<HttpConnectionHandler*> handlers;
  /// Chain of registered web applications. Each time an incoming HTTP request
  /// is received, the request is provided to each application of this chain.
  /// If an application detects the request is for it, the application will
  /// call the httpResponse.send() and the chain stops. If no web app serves
  /// the request, the not found page will be served.
  std::vector<HttpApp*> applications;
  /// @brief Creates a socket queue for the connection handlers to consume from
  Queue<Socket>* socketQueue;

 public:
    /**
    * @brief Constructs a new Http Server object with a singleton model.
    * @return HttpServer the instance of the server.
    *
    */
    static HttpServer& getInstance() {
      static HttpServer instance;
      return instance;
    }
    /**
    * @brief Registers a web application to the chain.
    * @param application is the web application to be registered.
    */
    void chainWebApp(HttpApp* application);
    /**
    * @brief start web server.
    * 
    * @param argc is the number of arguments.
    * @param argv is array of arguments.
    * @return int error code, 0 if no error.
    */
    int start(int argc, char* argv[]);
    /**
    * @brief Stop the web server.
    * @details The server may stop not immediately. It will stop listening for
    * further connection requests at once, but pending HTTP requests that are
    * enqueued will be allowed to finish.
    * @return int error code, 0 if no error.
    */
    void stop();
    /// Infinetelly listen for client connection requests and
    /// accept all of them.
    /// For each accepted connection request, the virtual onConnectionAccepted()
    /// will be called. Inherited classes must override that method
    void listenForever(const char* port);

 private:
    /**
     * @brief Analyze the command line arguments.
     * @param argc is the number of arguments.
     * @param argv is array of arguments.
     * @return true if program can continue execution, false otherwise.
     */
    bool analyzeArguments(int argc, char* argv[]);
    /// This method is called each time a client connection request is accepted.
    void handleClientConnection(Socket& client) override;
    /// Constructor
    HttpServer();
    /// Destructor
    ~HttpServer();
};
  #endif  // HTTPSERVER_H
