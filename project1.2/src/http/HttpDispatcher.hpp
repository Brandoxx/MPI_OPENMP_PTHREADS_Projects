// @copyright C_la_vie 2022
#include <string>
#include <vector>

#include "prodcons/Consumer.hpp"
#include "common/UtilStruct.hpp"

/**
 * @brief httpDispatcher is in charge of responding all completed requests.
 * it is a consumer of std::vector<gb_data_t*>.
 */
class HttpDispatcher : public Consumer<std::vector<gb_data_t*>> {
 public:
   /**
   * @brief Construct a new HttpDispatcher object.
   */
    HttpDispatcher() : Consumer<std::vector<gb_data_t*>>() {}
   /**
   * @brief Destruct a HttpDispatcher object.
   */
    ~HttpDispatcher() override {}
    /**
    * @brief overrides the consume method herited from Consumer. It consumes
    * from the vector of gb_data_t, one gb_data_t at a time and handles
    * the request.
    * @param data to be consumed.
    */
    void consume(std::vector<gb_data_t*> data) override;
   /**
   * @brief override the run method herited from Consumer and from Thread.
   * @return int with error code.
    */
    int run() override;

 private:
    /**
    * @brief Handle HTTP requests. @see HttpServer::handleHttpRequest()
    * @param data @see ClientRequest
    * @return true If this application handled the request, false otherwise
    * Sends the homepage as HTTP response
    */
    bool serveHomepage(clientRequest* data);

    /**
    * @brief Handle a HTTP request that starts with "/Goldbach"
    * @param httpRequest The HTTP request
    * @param httpResponse The HTTP response
    * @return true If this application handled the request, false otherwise
    * Sends the homepage as HTTP respons
    */
    bool serveGoldbach(HttpRequest& httpRequest, HttpResponse& httpResponse);

    /**
    * @brief Generate addends of all requests using @see Goldbach
    * @param httpRequest HTTP request from socket
    * @param httpResponse HTTP response that will be returned in the socket
    * @param solutions Vector will the numbers given by the user
    * @return error bool indicates if socket was succesfully send
    */
    bool serveGoldbachAddends(HttpRequest& httpRequest,
                              HttpResponse& httpResponse,
                              std::vector<gb_data_t*> solutions);

    /**
    * @brief Generate response of all output from Goldbach Addends
    * @param title Title of the page to be sent as a response
    * @param outputGoldbach Vector will the numbers given by the user
    * @return A string with the HTML design of the page
    */
    std::string serveGoldbachResponse(std::string title,
                                      std::vector<gb_data_t*> outputGoldbach);

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
    bool serveOutOfBounds(HttpRequest& httpRequest,
                          HttpResponse& httpResponse);
};
