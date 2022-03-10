#ifndef PARSER_H
#define PARSER_H
#include <request.h>

typedef enum ParseStatus{
  PARSE_OK,
  PARSE_DONE,
  PARSE_FALSE
} ParseStatus;

bool parse_first_line(HTTP_Request *req, char* buffer);

bool parse_headers(HTTP_Request *req, char* buffer, ParseStatus *s);

bool parse_request(HTTP_Request *req, char* buffer);

#endif