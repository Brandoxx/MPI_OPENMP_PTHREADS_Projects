// @copyright C_la_vie 2022
#include "HttpDispatcher.hpp"

int HttpDispatcher::run() {
  this->consumeForever();
  return 0;
}

bool HttpDispatcher::serveHomepage(clientRequest* request) {
  // Set HTTP response metadata (headers)
  request->response->setHeader("Server", "AttoServer v1.1");
  request->response->setHeader("Content-type", "text/html; charset=ascii");
  // Build the body of the response
  std::string title = "Goldbach Sums";
  request->response->body() << "<!DOCTYPE html>\n"
  << "<html lang=\"en\">\n"
  << "  <meta charset=\"ascii\"/>\n"
  << "  <title>" << title << "</title>\n"
  << "  <style>body {font-family: monospace}</style>\n"
  << "  <div style=\"text-align: center;\">\n"
  << "  <h1>" << title << "</h1>\n"
  << "  <form method=\"get\" action=\"/Goldbach\">\n"
  << "    <label for=\"number\">Number(s):</label>\n"
  << "    <input type=\"text\" name=\"number\" required/>\n"
  << "    <button type=\"submit\">Add Goldbach number</button>\n"
  << "  </form><br><hr color=\"black\">\n"
  << "  <p> App can be used by typing however many numbers, separated by ',',"
  << "   whose Goldbach Conjecture Sums you want to calculate and they will"
  << "   appear on screen.<br><br>It has to be a number between"
  << "   -9,223,372,036,854,775,808"
  << "   and 9,223,372,036,854,775,807.<br><br>"
  << "   The query with bigger numbers might take some time due to the amount"
  << "   of calculations that have to be done to calculate the sums of the"
  << "   given number.</p>\n<ul>\n"
  << "<li>Positive numbers show only the total sums of the number.</li>\n"
  << "<li>Negative numbers make the page show all the sums found of it's"
  << " positive counterpart.</li>\n</ul>\n<hr color=\"black\">\n"
  << "  </div>\n"
  << "</html>\n";
  // Send the response to the client (user agent)
  return request->response->send();
}

bool HttpDispatcher::serveGoldbachAddends(HttpRequest &httpRequest,
                                          HttpResponse &httpResponse,
                                          std::vector<gb_data_t*> solutions) {
  (void)httpRequest;
  std::string title = "Goldbach sums for ";
  //  Append all goldbach sums to body
  for (size_t writer = 0; writer < solutions.size(); writer++) {
    title = title + std::to_string(solutions.at(writer)->number);
    if (writer + 1 != solutions.size()) {
      title = title + ", ";
    }
  }
  httpResponse.body() << serveGoldbachResponse(title, solutions);
  return httpResponse.send();
}

std::string HttpDispatcher::serveGoldbachResponse(std::string title,
                                                  std::vector<gb_data_t*>
                                                  outputGoldbach) {
  // Adds body to the URI response
  std::stringstream response;
  response << "<!DOCTYPE html>\n"
           << "<html lang=\"en\">\n"
           << "  <meta charset=\"ascii\"/>\n"
           << "  <title>" << title << "</title>\n"
           << "  <style>body {font-family: monospace}\n"
           << "  .err {color: red}\n"
           << "  .column {column-width: 200px; column-gap: 40px;}\n"
           << "  </style>\n"
           << "  <div style=\"text-align: center;\">\n"
           << "  <h1>"
           << "Goldbach Sums"
           << "</h1>\n"
           << "  <form method=\"get\" action=\"/Goldbach\">\n"
           << "    <label for=\"number\">Number(s):</label>\n"
           << "    <input type=\"text\" name=\"number\" required/>\n"
           << "    <button type=\"submit\">Add Goldbach number</button>\n"
           << "  </form><br><hr color=\"black\">\n"
           << "  </div>\n"
           << "  <h2>" << title << "</h2>\n";
  // Adds each number to the URI to send
  for (auto &result : outputGoldbach) {
    if (abs(result->number) > 5 || abs(result->number) == 4) {
      response << "<h3>" << std::to_string(result->number) << "</h3>\n"
          << "<p>" << std::to_string(result->number) << ": "
          << std::to_string(result->totalAddends) << " sums</p>\n";
      if (result->number < 0) {
        response << "<hr color=\"black\"><hr color=\"black\">"
                 << "<ol>\n"
                 << "<div class = \"column\">\n";
        int indexer = result->format;
        for (size_t adder = 0; adder < result->goldbachAddends.size();
             adder += indexer) {
          response << "<li>";
          if (result->format == 2) {
            response << std::to_string(result->goldbachAddends[adder])
                << " + " << std::to_string(result->goldbachAddends[adder + 1])
                << "</li>\n";
          } else if (result->format == 3) {
            response << std::to_string(result->goldbachAddends[adder])
                << " + " << std::to_string(result->goldbachAddends[adder + 1])
                << " + " << std::to_string(result->goldbachAddends[adder + 2]);
          }
          response << "</li>\n";
        }
        response << "</div>\n"
                 << "</ol><hr color=\"black\">\n";
      }
    } else {
      response << "<h3 class=\"err\">"
               << std::to_string(result->number) << "</h3>\n"
               << "<p>" << std::to_string(result->number) << ": NA </p>\n";
    }
  }
  // Ends the URI and returns the response
  response << "  <div style=\"text-align: center;\">\n"
           << "<hr color=\"black\"><p><a href=\"/\">Homepage</a></p>\n"
           << "</div>\n"
           << "</html>\n";
  return response.str();
}

bool HttpDispatcher::serveInvalidRequest(HttpRequest &httpRequest,
                                         HttpResponse &httpResponse) {
  (void)httpRequest;
  // Build the body for an invalid request
  std::string title = "Invalid request";
  httpResponse.body() << "<!DOCTYPE html>\n"
      << "<html lang=\"en\">\n"
      << "  <meta charset=\"ascii\"/>\n"
      << "  <div style=\"text-align: center;\">\n"
      << "  <title>" << title << "</title>\n"
      << "  <style>body {font-family: monospace} .err {color: red}</style>\n"
      << "  <h1 class=\"err\">" << title << "</h1><hr color=\"black\">\n"
      << "  <p>Invalid request for Goldbach</p>\n"
      << "  <hr color=\"black\"><p><a href=\"/\">Homepage</a></p>\n"
      << "  </div>\n"
      << "</html>\n";
  return httpResponse.send();
}

bool HttpDispatcher::serveOutOfBounds(HttpRequest &httpRequest,
                                      HttpResponse &httpResponse) {
  (void)httpRequest;
  // Build the body for an invalid request
  std::string title = "Invalid request";
  httpResponse.body() << "<!DOCTYPE html>\n"
      << "<html lang=\"en\">\n"
      << "  <meta charset=\"ascii\"/>\n"
      << "  <div style=\"text-align: center;\">\n"
      << "  <title>" << title << "</title>\n"
      << "  <style>body {font-family: monospace} .err {color: red}</style>\n"
      << "  <h1 class=\"err\">" << title << "</h1><hr color=\"black\">\n"
      << "  <p>The requested number was out of range.</p>\n"
      << "  <p>Accepted range: -9.223.372.036.854.775.808 to "
      << "  9.223.372.036.854.775.807.</p>\n"
      << "  <hr color=\"black\"><p><a href=\"/\">Homepage</a></p>\n"
      << "  </div>\n"
      << "</html>\n";
  return httpResponse.send();
}

void HttpDispatcher::consume(std::vector<gb_data_t*> data) {
  //  If request is valid
  if (data.at(0)->packetTotal > 0) {
    this->serveGoldbachAddends(*data[0]->requestID->request,
     *data[0]->requestID->response, data);
  //  If request is invalid
  } else if (data.at(0)->packetTotal == -1) {
    this->serveInvalidRequest(*data[0]->requestID->request,
     *data[0]->requestID->response);
  //  If request is out of bounds
  } else if (data.at(0)->packetTotal == -2) {
    this->serveOutOfBounds(*data[0]->requestID->request,
     *data[0]->requestID->response);
  //  If request is for homepage
  } else if (data.at(0)->packetTotal == -3) {
    this->serveHomepage(data[0]->requestID);
  }
  //  Frees memory of the recently consumed gb_data_t*
  delete data[0]->requestID->request;
  delete data[0]->requestID->response;
  delete data[0]->requestID;
  for (auto &result : data) {
    delete result;
  }
}
