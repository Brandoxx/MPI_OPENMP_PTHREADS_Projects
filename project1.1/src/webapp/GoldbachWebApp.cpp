// @copyright C_la_vie 2022

#include <algorithm>
#include <cassert>
#include <iostream>
#include <regex>
#include <stdexcept>

#include "GoldbachWebApp.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

GoldbachWebApp::GoldbachWebApp() {
}

GoldbachWebApp::~GoldbachWebApp() {
}

void GoldbachWebApp::start() {
  // TODO(you): Start producers, consumers, assemblers...
}

void GoldbachWebApp::stop() {
  // TODO(you): Stop producers, consumers, assemblers...
}

bool GoldbachWebApp::handleHttpRequest(HttpRequest& httpRequest,
                                       HttpResponse& httpResponse) {
  // If the home page was asked
  if (httpRequest.getMethod() == "GET" && httpRequest.getURI() == "/") {
    return this->serveHomepage(httpRequest, httpResponse);
  }
  // If the request starts with "Goldbach/" is for this web app
  if (httpRequest.getURI().rfind("/Goldbach", 0) == 0) {
    // serve goldbach here!
    return this->serveGoldbach(httpRequest, httpResponse);
  }
  // Unrecognized request
  return false;
}

std::pair<int, std::vector<int64_t>> GoldbachWebApp::analyzeURI(
                                                              std::string URI) {
  std::pair<int, std::vector<int64_t>> reaction = {0, std::vector<int64_t>()};
  for (size_t index = 0; index < URI.length(); index++) {
    if (URI[index] == ',') {
      URI.replace(index, 1, "%2C");
    }
  }
  std::smatch matches;
  std::regex inQuery("^/Goldbach(/|\\?number=)([+-]?\\d+(%2C[+-]?\\d+)*)$");
  if (std::regex_search(URI, matches, inQuery)) {
    assert(matches.length() >= 3);
    std::string numberString = matches[2];
    size_t numberEndIndex = 0;
    std::string tempString = "";
    while ((numberEndIndex = numberString.find("%2C")) != std::string::npos) {
      tempString = numberString.substr(0, numberEndIndex);
      try {
        (reaction.second).push_back(std::stoll(numberString));
      } catch(std::out_of_range const& ex) {}
      numberString.erase(0, numberEndIndex + 3);
    }
    try {
      (reaction.second).push_back(std::stoll(numberString));
    } catch(std::out_of_range const& ex) {}
    if ((reaction.second).empty()) {
      reaction.first = 1;
    } else {
      reaction.first = 2;
    }
  } else {
    reaction.first = 0;
  }
  return reaction;
}

bool GoldbachWebApp::serveHomepage(HttpRequest& httpRequest,
                                   HttpResponse& httpResponse) {
  (void)httpRequest;

  // Set HTTP response metadata (headers)
  httpResponse.setHeader("Server", "AttoServer v1.1");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");

  // Build the body of the response
  std::string title = "Goldbach Sums";
  httpResponse.body() << "<!DOCTYPE html>\n"
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
  return httpResponse.send();
}

bool GoldbachWebApp::serveGoldbach(HttpRequest& httpRequest,
                                   HttpResponse& httpResponse) {
  (void)httpRequest;
  // Set HTTP response metadata (headers)
  httpResponse.setHeader("Server", "AttoServer v1.0");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");
  // If a number was asked in the form "/Goldbach/1223"
  // or "/Goldbach?number=1223"
  std::string URI = httpRequest.getURI();
  // Send the response to the client (user agent)
  bool response = false;
  std::pair<int, std::vector<int64_t>> reaction = this->analyzeURI(URI);
  if (reaction.first == 0) response = this->serveInvalidRequest(httpRequest,
                                            httpResponse);
  else if (reaction.first == 1) response = this->serveOutOfBounds(httpRequest,
                                                 httpResponse);
  else if (reaction.first == 2) response = this->serveGoldbachAddends(
                                                 httpRequest, httpResponse,
                                                 reaction.second);
  return response;
}

bool GoldbachWebApp::serveGoldbachAddends(HttpRequest& httpRequest,
                                          HttpResponse& httpResponse,
                                          std::vector<int64_t> inputGoldbach) {
  (void)httpRequest;
  std::vector<Goldbach::gb_data_t> outputGoldbach = goldbach.goldbachMaker(
                                                             inputGoldbach);
  std::string title = "Goldbach sums for ";
  for (size_t writer = 0; writer < inputGoldbach.size(); writer++) {
    title = title + std::to_string(inputGoldbach.at(writer));
    if (writer + 1 != inputGoldbach.size()) {
      title = title + ", ";
    }
  }
  httpResponse.body() << serveGoldbachResponse(title, outputGoldbach);
  return httpResponse.send();
}

std::string GoldbachWebApp::serveGoldbachResponse(std::string title,
                              std::vector<Goldbach::gb_data_t> outputGoldbach) {
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
    << "  <h1>" << "Goldbach Sums" << "</h1>\n"
    << "  <form method=\"get\" action=\"/Goldbach\">\n"
    << "    <label for=\"number\">Number(s):</label>\n"
    << "    <input type=\"text\" name=\"number\" required/>\n"
    << "    <button type=\"submit\">Add Goldbach number</button>\n"
    << "  </form><br><hr color=\"black\">\n"
    << "  </div>\n"
    << "  <h2>" << title << "</h2>\n";
  for (auto &result : outputGoldbach) {
    if (abs(result.number) > 5 || abs(result.number) == 4) {
      response
      << "<h3>" << std::to_string(result.number) << "</h3>\n"
      << "<p>" << std::to_string(result.number) << ": "
      << std::to_string(result.totalAddends) << " sums</p>\n";
      if (result.number < 0) {
        response << "<hr color=\"black\"><hr color=\"black\">"
        << "<ol>\n" << "<div class = \"column\">\n";
        int indexer = result.format;
        for (size_t adder = 0; adder < result.goldbachAddends.size();
            adder+=indexer) {
          response << "<li>";
          if (result.format == 2) {
              response << std::to_string(result.goldbachAddends[adder])
              << " + " << std::to_string(result.goldbachAddends[adder+1])
              << "</li>\n";
          } else if (result.format == 3) {
              response << std::to_string(result.goldbachAddends[adder])
              << " + " << std::to_string(result.goldbachAddends[adder+1])
              << " + " << std::to_string(result.goldbachAddends[adder+2]);
          }
          response << "</li>\n";
        }
        response << "</div>\n" << "</ol><hr color=\"black\">\n";
      }
    } else {
      response << "<h3 class=\"err\">"
      << std::to_string(result.number) << "</h3>\n"
      << "<p>" << std::to_string(result.number) << ": NA </p>\n";
    }
  }
  response << "  <div style=\"text-align: center;\">\n"
  << "<hr color=\"black\"><p><a href=\"/\">Homepage</a></p>\n"
  << "</div>\n" << "</html>\n";
  return response.str();
}

bool GoldbachWebApp::serveInvalidRequest(HttpRequest& httpRequest,
                                          HttpResponse& httpResponse) {
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

bool GoldbachWebApp::serveOutOfBounds(HttpRequest& httpRequest,
                                      HttpResponse& httpResponse) {
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
