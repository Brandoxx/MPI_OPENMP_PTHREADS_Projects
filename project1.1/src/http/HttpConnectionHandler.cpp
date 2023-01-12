// @copyright C_la_vie 2022

#include "HttpConnectionHandler.hpp"

HttpConnectionHandler::HttpConnectionHandler() {
}
HttpConnectionHandler::~HttpConnectionHandler() {
}
void HttpConnectionHandler::setApplications(
  std::vector<HttpApp*> applications) {
    this->applications = applications;
}
void HttpConnectionHandler::consume(Socket client) {
    while (true) {
      HttpRequest httpRequest(client);
      // If the request is not valid or an error happened
      if (!httpRequest.parse()) {  // get the request from the client
        // Non-valid requests are normal after a previous valid request. Do not
        // close the connection yet, because the valid request may take time to
        // be processed. Just stop waiting for more requests
        break;  // if the request is not valid, it will break the loop
      }
      // A complete HTTP client request was received. Create an object for the
      // server responds to that client's request
      HttpResponse httpResponse(client);  // generates the response
      // Give subclass a chance to respond the HTTP request
      // this give part to the subclass in charge of responding the request
      const bool handled = handleHttpRequest(httpRequest, httpResponse);
      // If subclass did not handle the request or the client used HTTP/1.0
      // this question is do i have to close the connection or not
      if (!handled || httpRequest.getHttpVersion() == "HTTP/1.0") {
        // The socket will not be more used, close the connection
        client.close();
        break;
      }
    }
  }
/// ---HANDLE REQUEST WILL BE THE JOB FOR THE HANDLERS---///
bool HttpConnectionHandler::handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse) {
  // Print IP and port from client
  const NetworkAddress& address = httpRequest.getNetworkAddress();
  Log::append(Log::INFO, "connection",
    std::string("connection established with client ") + address.getIP()
    + " port " + std::to_string(address.getPort()));

  // Print HTTP request
  // reports what client is asking for
  Log::append(Log::INFO, "request", httpRequest.getMethod()
    + ' ' + httpRequest.getURI()
    + ' ' + httpRequest.getHttpVersion());
  // send the request to the corresponding webapp
  return route(httpRequest, httpResponse);
}
bool HttpConnectionHandler::route(HttpRequest& httpRequest,
  HttpResponse& httpResponse) {
  //  Traverse the chain of applications
  std::cout << "enrouting" << std::endl;
  for (size_t index = 0; index < this->applications.size(); ++index) {
    //  If this application handles the request
    //  i ask "is this the kind of request you can handle?"
    HttpApp* app = this->applications[index];
    if (app->handleHttpRequest(httpRequest, httpResponse)) {
      return true;
    }
  }
  // Unrecognized request
  return serveNotFound(httpRequest, httpResponse);
}
bool HttpConnectionHandler::serveNotFound(HttpRequest& httpRequest
  , HttpResponse& httpResponse) {
  (void)httpRequest;
  //  Set HTTP response metadata (headers)
  httpResponse.setStatusCode(404);  //  not found, 200 is ok,300 is redirect
  //  400 is bad request, 500 is internal server error, 100 is information.
  httpResponse.setHeader("Server", "AttoServer v1.0");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");
  //  Build the body of the response
  std::string title = "Not found";
  httpResponse.body() << "<!DOCTYPE html>\n"
    << "<html lang=\"en\">\n"
    << "  <meta charset=\"ascii\"/>\n"
    << "  <title>" << title << "</title>\n"
    << "  <style>body {font-family: monospace} h1 {color: red}</style>\n"
    << "  <h1>" << title << "</h1>\n"
    << "  <p>The requested resouce was not found on this server.</p>\n"
    << "  <hr color=\"black\"><p><a href=\"/\">Homepage</a></p>\n"
    << "</html>\n";
  // Send the response to the client (user agent)
  return httpResponse.send();
}
int HttpConnectionHandler::run() {
  this->consumeForever();
  return EXIT_SUCCESS;
}

