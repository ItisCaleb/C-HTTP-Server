#ifndef REQUEST_H
#define REQUEST_H
#include <string.h>
#include <stdbool.h>
#include <response.h>

#define REQUEST_LINE_MAX_LEN 8192

#define methods \
METHOD(GET, 0) \
METHOD(HEAD, 1) \
METHOD(POST, 2) \
METHOD(PUT, 3) \
METHOD(DELETE, 4) \
METHOD(CONNECT, 5) \
METHOD(OPTIONS, 6) \
METHOD(TRACE, 7) \


#define METHOD(name, value) \
    name = value,

typedef enum {
    methods
} HTTP_method;

#undef METHOD

#define METHOD(name, value) \
    #name,

extern char* constants[];  

typedef struct HTTP_Request{
    HTTP_method method;
    char *path;
    float HTTP_VERSION;
    struct hashmap *headers;
    char *params;
    char *data;
    HTTP_Status _status;
} HTTP_Request;

HTTP_Request *create_request(char* buffer);

char* req_get_method(HTTP_Request* req);

float req_get_version(HTTP_Request* req);

char* req_get_path(HTTP_Request* req);

char* req_get_data(HTTP_Request* req);

void free_request(HTTP_Request *req);

#endif