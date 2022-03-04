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
METHOD(TRACE, 6) \


#define METHOD(name, value) \
    name = value,

typedef enum {
    methods
} HTTP_method;

#undef METHOD

#define METHOD(name, value) \
    #name,

extern const char* constants[];  


typedef struct HTTP_Request{
    HTTP_method method;
    char *path;
    float HTTP_VERSION;
    struct hashmap *headers;
    char *data;
    bool _parse_ok;
    HTTP_Status _status;
} HTTP_Request;

HTTP_Request *create_request(char* buffer);

#endif