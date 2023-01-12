// @copyright C_la_vie 2022

#include "HttpConnectionHandler.hpp"

void HttpConnectionHandler::setOwnClientRequests() {
  this->clientRequests = new Queue<clientRequest*>();
}
void HttpConnectionHandler::setClientRequests(
  Queue<clientRequest*>* clientRequests) {
  this->clientRequests = clientRequests;
}
Queue<clientRequest*>* HttpConnectionHandler::getClientRequests() {
  return this->clientRequests;
}
void HttpConnectionHandler::setApplications(
  std::vector<HttpApp*> applications) {
    this->applications = applications;
}
bool HttpConnectionHandler::enqueueClientRequest(HttpRequest* request,
  HttpResponse* response) {
    clientRequest* newRequest = new clientRequest();
    newRequest->request = request;
    newRequest->response = response;
    this->clientRequests->push(newRequest);
    return true;
}

void HttpConnectionHandler::consume(Socket client) {
    while (true) {
      HttpRequest* httpRequest = new HttpRequest(client);
      // If the request is not valid or an error happened
      if (!httpRequest->parse()) {  // get the request from the client
        // Non-valid requests are normal after a previous valid request. Do not
        // close the connection yet, because the valid request may take time to
        // be processed. Just stop waiting for more requests
        delete httpRequest;
        break;  // if the request is not valid, it will break the loop
      }
      // A complete HTTP client request was received. Create an object for the
      // server responds to that client's request. Generates the response
      HttpResponse* httpResponse = new HttpResponse(client);
      // Give subclass a chance to respond the HTTP request
      // we make a client request struct and we put it in the queue
      const bool handled = handleHttpRequest(httpRequest, httpResponse);
      if (!handled || httpRequest->getHttpVersion() == "HTTP/1.0") {
        // The socket will not be more used, close the connection
        client.close();
        break;
      }
      // this give part to the subclass in charge of responding the request
      // const bool handled = handleHttpRequest(httpRequest, httpResponse);
      // // If subclass did not handle the request or the client used HTTP/1.0
      // // this question is do i have to close the connection or not
      // if (!handled || httpRequest.getHttpVersion() == "HTTP/1.0") {
      //   // The socket will not be more used, close the connection
      //   client.close();
      //   break;
      // }
    }
  }
bool HttpConnectionHandler::handleHttpRequest(HttpRequest* httpRequest,
    HttpResponse* httpResponse) {
  // Print IP and port from client
  const NetworkAddress& address = httpRequest->getNetworkAddress();
  Log::append(Log::INFO, "connection",
    std::string("connection established with client ") + address.getIP()
    + " port " + std::to_string(address.getPort()));

  // Print HTTP request
  // reports what client is asking for
  Log::append(Log::INFO, "request", httpRequest->getMethod()
    + ' ' + httpRequest->getURI()
    + ' ' + httpRequest->getHttpVersion());
  // send the request to the corresponding webapp
  return enqueueClientRequest(httpRequest, httpResponse);
}
int HttpConnectionHandler::run() {
  this->consumeForever();
  this->clientRequests->push(nullptr);
  return EXIT_SUCCESS;
}

