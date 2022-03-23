#include <headers.h>
#include <parser.h>
#include <request.h>
#include <stdio.h>
#include <stdlib.h>

HTTP_Request *create_request(char *buffer) {
  HTTP_Request *req = calloc(1, sizeof(HTTP_Request));
  req->path = calloc(1, REQUEST_LINE_MAX_LEN);
  req->headers = create_headers();
  req->_status = OK;
  parse_request(req, buffer);
  return req;
}

char* req_get_method(HTTP_Request *req) {
  if (!req) {
    printf("You can't pass NULL request!");
    return 0;
  }
  return constants[req->method];
}

float req_get_version(HTTP_Request *req) {
  if (!req) {
    printf("You can't pass NULL request!");
    return 0;
  }
  return req->HTTP_VERSION;
}

char *req_get_path(HTTP_Request *req) {
  if (!req) {
    printf("You can't pass NULL request!");
    return NULL;
  }
  return req->path;
}

char *req_get_data(HTTP_Request *req) {
  if (!req) {
    printf("You can't pass NULL request!");
    return NULL;
  }
  return req->data;
}

void free_request(HTTP_Request *req) {
  if (!req) {
    printf("You can't pass NULL request!");
    return;
  }
  hashmap_free(req->headers);
  free(req->path);
  if (req->data != NULL) {
    free(req->data);
  }
  free(req);
}