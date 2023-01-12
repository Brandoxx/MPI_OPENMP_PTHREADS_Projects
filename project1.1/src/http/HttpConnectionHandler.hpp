/**
 * @author A.Badilla Olivas
 * @author B.Mora Umana
 * @author G. Molina Burgarelli
 * @author A. Madriz Aguero 
 * @version 0.1
 * @date 2022-10-11
 * 
 * @copyright // Copyright  2022  GNU GENERAL PUBLIC LICENSE
 */

#ifndef HTTP_CONNECTION_HANDLER_HPP
#define HTTP_CONNECTION_HANDLER_HPP

#include <vector>
#include <string>
#include "prodcons/Consumer.hpp"
#include "network/Socket.hpp"
#include "common/common.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "common/Log.hpp"
#include "network/NetworkAddress.hpp"
#include "HttpApp.hpp"

/**
* @brief HTTPconnection handler handles all connections from a single client.
* @details currently, this class is just a consumer of sockets.
* It resolves the requests and sends the responses.
* @param applications are the applications that will be used to handle requests.
*/
class HttpConnectionHandler
  : public Consumer<Socket> {
    /*! \cond PRIVATE */
    /// Disable default methods for copying objects.
    DISABLE_COPY(HttpConnectionHandler);
    /*! \endcond */

 protected:
  /// Vector of applications to be used in the webserver
  std::vector<HttpApp*> applications;

 public:
  /**
   * @brief Construct a new Http Connection Handler object.
   *
   */
    HttpConnectionHandler();
  /**
   * @brief Destroy the Http Connection Handler object.
   *
   */
    ~HttpConnectionHandler();
  /**
   * @brief override the run method herited from Consumer and from Thread.
   * 
   * @return int with error code.
   */
    int run() override;
  /**
   * @brief overrides the consume method herited from Consumer. It consumes
   * it consumes from the queue of sockets one socket at a time and handles
   * the request.
   * @param socket to be consumed.
   */
    void consume(Socket socket) override;
  /**
   * @brief Set the Applications object.
   *
   * @param applications the applications to be used to handle requests.
   */
    void setApplications(std::vector<HttpApp*> applications);
 private:
    /**
     * @brief handles a request from a client.
     * 
     * @param httpRequest with the request.
     * @param httpResponse to fill with the response.
     * @return bool if the request was handled, false otherwise.
     */
    bool handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse);
     /** 
      * @brief Route the request to the correct application.
      * @param httpRequest the request to be routed.
      * @param httpResponse the response filled by the application.
     */
    bool route(HttpRequest& httpRequest, HttpResponse& httpResponse);
    /**
     * @brief Sends a page for a non found resouce in this server.
     * @details
     * This method is called
     * if none of the registered web applications handled the request.
     * that reacts to all URIs, and chain it as the last web app.
     * @param httpRequest to be handled.
     * @param httpResponse to respond.
     * @return bool true if the request was handled, false otherwise.
     */
    bool serveNotFound(HttpRequest& httpRequest, HttpResponse& httpResponse);
};
#endif  //  HTTP_CONNECTION_HANDLER_HPP
