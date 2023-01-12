// @copyright C_la_vie 2022

#ifndef UTIL_STRUCT_HPP
#define UTIL_STRUCT_HPP

#include <stdint.h>
#include <vector>
#include "http/HttpResponse.hpp"
#include "http/HttpRequest.hpp"

/**
 * @brief This structs contains the request and the response of a client.
 * @param request: the request of the client.
 * @param response: the response of the client.
 */
typedef struct {
  HttpRequest* request;
  HttpResponse* response;
} clientRequest;

/** @struct gb_data_t
*  @brief This structure stores data required to calculate goldbachAddends
*  @param requestID
*  Member to identify source client
* @param number
*  Member number contains of reference to get goldbach series
*  @param format
*  Member format contains a reference expressing how to read goldbach series
*  @param totalAddends
*  Member totalAddends contains the total number of series found
*  @param goldbachAddends
*  Member goldbachAddends contains all the goldbach series foun
*/
typedef struct {
  clientRequest* requestID;
  int packetTotal;
  int number;
  int format;
  int totalAddends;
  std::vector<int64_t> goldbachAddends;
} gb_data_t;

#endif  // UTIL__HPP
