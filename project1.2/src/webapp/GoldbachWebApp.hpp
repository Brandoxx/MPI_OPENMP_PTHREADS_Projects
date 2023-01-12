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

#ifndef GOLDBACHWEBAPP_HPP
#define GOLDBACHWEBAPP_HPP

#include <string>
#include <utility>
#include <vector>

#include "HttpApp.hpp"
#include "GoldbachCalculator.hpp"  // Contains all methods for Goldbach calculations
#include "Dispatcher.hpp"
#include "common/UtilStruct.hpp"
#include "GoldbachSolvers.hpp"
#include "GoldbachPacker.hpp"
#include "http/HttpDispatcher.hpp"

/** @class GoldbachWebApp
* @brief A web application that calculates Goldbach Conjecture Sums
*/
class GoldbachWebApp : public HttpApp {
  /*! \cond PRIVATE */
  /// Objects of this class cannot be copied
  DISABLE_COPY(GoldbachWebApp);
  /*! \endcond */

 private:
  /// Goldbach object that makes the sums of whichever numbers it's given

  /// Goldbach packer as part of assembly line
  /// Gather all gb_data_t from the same source
  GoldbachPacker* packer;

  /// Golbach dispatcher as part of assembly line
  /// Send the response to client
  HttpDispatcher* dispatcher;

  /// Amount of solver to create
  size_t solverCount;

  /// Goldbach Solver as part of assembly line
  /// Found all goldbach series found from an entry
  std::vector<GoldbachSolvers*> solvers;

  /// Input queue for GoldbachSolvers
  Queue<gb_data_t*>* solverInputQueue;

  /// Output queue for GoldbachSolvers
  Queue<gb_data_t*>* solverOutputQueue;

  /// Output queue for GoldbachPacker
  Queue<std::vector<gb_data_t*>>* packerOutputQueue;

 public:
  /// Constructor
  GoldbachWebApp() : HttpApp() {}
  /// Destructor
  ~GoldbachWebApp() {}
  /// Override the run method herited from Consumer and from Thread.
  int run() override;
  /// Called by the web server when the web server is started
  void start() override;
  /**
  * @brief Handle HTTP requests. @see HttpServer::handleHttpRequest()
  * @param request The client request with the HTTP request and http response
  * @return true If this application handled the request, false otherwise
  * and another chained application should handle it
  */
  bool handleClientRequest(clientRequest* request) override;

  /**
   * @brief Re-definition of consume
   * this consume method call produce for assembly definition
   * @param request* struct contains httpRequest and httpResponse
   */
  void consume(clientRequest* request) override;

  /**
  * @brief Re-definition of consumeForever
  * Once recieved stopCondition destroy all assembly line
  */
  void consumeForever() override;

  /// Called when the web server stops, in order to allow the web application
  /// clean up and finish as well
  void stop() override;

  /**
   * @brief Set the Own Producing Queue object
   */
  void setOwnProducingQueue();
  Queue<gb_data_t*>* getSolversOutputQueue();

  /**
   * @brief Get the instance object of GoldbachWebapp
   * @return GoldbachWebApp& as a single instance of an application
   */
  static GoldbachWebApp& getInstance() {
    static GoldbachWebApp instance;
    return instance;
  }

 protected:
  // void produce(gb_data_t* number) override;
   /**
   * @brief Analyzes the URI given by the request given by the user
   * @param URI String with the numbers given by the user
   * @return Vector with the numbers found, if it finds one, and the code of the page to be sent
   */
  std::pair<int, std::vector<int64_t>> analyzeURI(std::string URI);

  /**
  * @brief Handle a HTTP request that starts with "/Goldbach"
  * @param request* struct contains httpRequest and httpResponse
  * @return true if Goldbach was handled, false if it must be
  * handled by another application
  */
  bool serveGoldbach(clientRequest* request);
  /**
   * @brief serves the main page of the web application
   * 
   * @param request 
   * @return true  
   */
  bool serveHomePage(clientRequest* request);
};

#endif  // GoldbachWEBAPP_HPP
