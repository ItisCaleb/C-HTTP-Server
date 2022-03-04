#ifndef HTTP_SERVER
#define HTTP_SERVER

#include <tcpserver.h>
#include <routes.h>

typedef struct HTTP_Server{
    unsigned short port;
    struct hashmap *routes;
    TCP_Server *_tcp_server;
} HTTP_Server;

HTTP_Server* create_server(unsigned short port);

void handle_route(HTTP_Server* server, char* path, Request_Handler handler);

void handle_response(HTTP_Server* server, HTTP_Response* res);

void start(HTTP_Server* server);

#endif

