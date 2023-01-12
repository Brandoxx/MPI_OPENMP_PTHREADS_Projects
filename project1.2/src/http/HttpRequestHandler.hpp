#ifndef HTTP_REQUEST_HANDLER_HPP
#define HTTP_REQUEST_HANDLER_HPP

#include <map>
// @copyright C_la_vie 2022
#include <string>

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "HttpConnectionHandler.hpp"
#include "prodcons/Dispatcher.hpp"
#include "common/UtilStruct.hpp"

class HttpRequestHandler: public Consumer <clientRequest*> {
 public:
    /**
    * @brief Construct a new HttpDispatcher object.
    */
    HttpRequestHandler(): Consumer<clientRequest*>() {}
    /**
    * @brief Destruct a HttpDispatcher object.
    */
    ~HttpRequestHandler() {}
    /**
    * @brief overrides the consumeForever method herited from Consumer to
    * consume with consume()
    */
    void consumeForever() override;
    /**
    * @brief overrides the consume method herited from Consumer. It consumes
    * from the vector of clientRequest, one clientRequest at a time and handles
    * the request.
    * @param data to be consumed.
    */
    void consume(clientRequest* data) override;
    /**
    * @brief Connects a key to a specific queue.
    * @param key to be used to find the queue.
    * @param toQueue which key is aligned to.
    */
    void registerRedirect(int key, Queue<clientRequest*>* toQueue) {
      // here it creates its map of queues to send data to the consumers
      // through the keys
      this->toQueues[key] = toQueue;
    }

 private:
   /**
   * @brief This method is called by the dispatcher when a new request is received.
   * It extracts the key from the request, and it places the response.
   * @param data: the request received.
   * @return The response to the request.
   */
    int extractKey(const clientRequest* data);
    /// @brief Map of queues to send data to the consumers through the keys
    std::map<int, Queue<clientRequest*>*> toQueues;

 protected:
  /** 
  * @brief This method is assigned to the thread in charge of dispatching the requests.
  */
  int run() override;
};

#endif  // HTTP_REQUEST_HANDLER_HPP
