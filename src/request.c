#include <request.h>
#include <stdio.h>
#include <parser.h>
#include <stdlib.h>
#include <headers.h>


HTTP_Request* create_request(char* buffer) {
  HTTP_Request *req = calloc(1, sizeof(HTTP_Request));
  req->path = calloc(1, REQUEST_LINE_MAX_LEN);
  req->headers = create_headers();
  req->_parse_ok = parse_request(req,buffer);
  return req;
}

HTTP_method req_get_method(HTTP_Request* req){
  if(!req){
    printf("You can't pass NULL request!");
    return 0;
  }
  return req->method;
}

char* req_get_path(HTTP_Request* req){
  if(!req){
    printf("You can't pass NULL request!");
    return NULL;
  }
  return req->path;
}

void free_request(HTTP_Request *req) {
  if(!req){
    printf("You can't pass NULL request!");
    return;
  }
  hashmap_free(req->headers);
  free(req->path);
  if(req->data != NULL){
    free(req->data);
  }
  free(req);
}