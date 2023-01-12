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

/** @class GoldbachWebApp
* @brief A web application that calculates Goldbach Conjecture Sums
*/
class GoldbachWebApp : public HttpApp {
  /*! \cond PRIVATE */
  /// Objects of this class cannot be copied
  DISABLE_COPY(GoldbachWebApp);
  /*! \endcond */

 private:
  /// Goldbach object that makes the sums of whichever numbers its given
  Goldbach goldbach;

 public:
  /// Constructor
  GoldbachWebApp();
  /// Destructor
  ~GoldbachWebApp();
  /// Called by the web server when the web server is started
  void start() override;

  /**
  * @brief Handle HTTP requests. @see HttpServer::handleHttpRequest()
  * @param httpRequest HTTP request from socket
  * @param httpResponse HTTP response that will be returned in the socket
  * @return true If this application handled the request, false otherwise
  * and another chained application should handle it
  */
  bool handleHttpRequest(HttpRequest& httpRequest,
                         HttpResponse& httpResponse) override;

  /// Called when the web server stops, in order to allow the web application
  /// clean up and finish as well
  void stop() override;

 protected:
   /**
   * @brief Analyzes the URI given by the request given by the user
   * @param URI String with the numbers given by the user
   * @return Vector with the numbers found, if it finds one, and the code of the page to be sent
   */
  std::pair<int, std::vector<int64_t>> analyzeURI(std::string URI);

  /**
  * @brief Handle HTTP requests. @see HttpServer::handleHttpRequest()
  * @param httpRequest HTTP request from socket
  * @param httpResponse HTTP response that will be returned in the socket
  * @return true If this application handled the request, false otherwise
  * Sends the homepage as HTTP response
  */
  bool serveHomepage(HttpRequest& httpRequest, HttpResponse& httpResponse);

  /**
  * @brief Handle a HTTP request that starts with "/Goldbach"
  * @param httpRequest HTTP request from socket
  * @param httpResponse HTTP response that will be returned in the socket
  * @return true if Goldbach was handled, false if it must be
  * handled by another application
  */
  bool serveGoldbach(HttpRequest& httpRequest, HttpResponse& httpResponse);

  /**
  * @brief Generate addends of all requests using @see Goldbach
  * @param httpRequest HTTP request from socket
  * @param httpResponse HTTP response that will be returned in the socket
  * @param inputGoldbach Vector will the numbers given by the user
  * @return error bool indicates if socket was succesfully send
  */
  bool serveGoldbachAddends(HttpRequest& httpRequest,
                            HttpResponse& httpResponse,
                            std::vector<int64_t> inputGoldbach);

  /**
  * @brief Generate response of all output from Goldbach Addends
  * @param title Title of the page to be sent as a response
  * @param outputGoldbach Vector will the numbers given by the user
  * @return A string with the HTML design of the page
  */
  std::string serveGoldbachResponse(std::string title,
                             std::vector<Goldbach::gb_data_t> outputGoldbach);

  /**
  * @brief Generate response in case input not match with regex
  * @param httpRequest HTTP request from socket
  * @param httpResponse HTTP response that will be returned in the socket
  * @return error bool indicates if socket was succesfully send
  */
  bool serveInvalidRequest(HttpRequest& httpRequest,
                           HttpResponse& httpResponse);

  /**
  * @brief Generate response in case some input in stream is invalid
  * @param httpRequest HTTP request from socket
  * @param httpResponse HTTP response that will be returned in the socket
  * @return error bool indicates if socket was succesfully send
  */
  bool serveOutOfBounds(HttpRequest& httpRequest, HttpResponse& httpResponse);
};

#endif  // GoldbachWEBAPP_HPP
