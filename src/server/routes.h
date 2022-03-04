#ifndef ROUTES_H
#define ROUTES_H

#include <request.h>
#include <response.h>

typedef void (*Request_Handler)(HTTP_Request* req, HTTP_Response* res);

typedef struct HTTP_Route{
    char* path;
    Request_Handler handler;
} HTTP_Route;

struct hashmap* create_router();

void add_route(struct hashmap *routes, char *path, Request_Handler handler);

bool execute_handler(struct hashmap *routes, char*path, HTTP_Request* req, HTTP_Response* res);

#endif