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
#include "common/UtilStruct.hpp"

/**
* @brief HTTPconnection handler handles all connections from a single client.
* @details currently, this class is just a consumer of sockets.
* It resolves the requests and sends the responses.
* @param applications are the applications that will be used to handle requests.
*/
class HttpConnectionHandler: public Consumer<Socket> {
    /*! \cond PRIVATE */
    /// Disable default methods for copying objects.
    DISABLE_COPY(HttpConnectionHandler);
    /*! \endcond */

 protected:
  /// Vector of applications to be used in the webserver
  std::vector<HttpApp*> applications;
  // A queue of client requests
  Queue<clientRequest*>* clientRequests;

 public:
  /**
   * @brief Construct a new Http Connection Handler object.
   */
    HttpConnectionHandler() {
    }
  /**
   * @brief Destroy the Http Connection Handler object.
   */
    ~HttpConnectionHandler() {
    }
  /**
   * @brief override the run method herited from Consumer and from Thread.
   * @return int with error code.
   */
    int run() override;
    /**
    * @brief overrides the consume method herited from Consumer. It consumes
    * from the queue of sockets one socket at a time and handles the request.
    * @param socket to be consumed.
    */
    void consume(Socket socket) override;
    /**
     * @brief Set the Applications object.
     *
     * @param applications the applications to be used to handle requests.
     */
    void setApplications(std::vector<HttpApp*> applications);

    /**
     * @brief sets the queue of client requests.
     * 
     */
    void setOwnClientRequests();
    /**
     * @brief sets the queue of client requests.
     */
    void setClientRequests(Queue<clientRequest*>* clientRequests);

    /**
     * @brief gets the queue of client requests.
     * @return Queue<ClientRequest*> 
     */
    Queue<clientRequest*>* getClientRequests();

 private:
  /**
   * @brief handles a client request.
   * 
   * @param httpRequest the request to be handled.
   * @param httpResponse the response to be fill
   * @return true if the request was handled.
   */
  bool handleHttpRequest(HttpRequest* httpRequest, HttpResponse* httpResponse);

  /**
   * 
   * @brief builds a ClientRequest struct from a socket and
   * adds it to the queue of client requests.
   * @param request to be handled
   * @param response to be sent
   * @return int boolean value indicating if the request was added to the
   * queue.
  */
  bool enqueueClientRequest(HttpRequest* request, HttpResponse* response);
};
#endif  //  HTTP_CONNECTION_HANDLER_HPP
