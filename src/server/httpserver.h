#ifndef HTTP_SERVER
#define HTTP_SERVER

#include <tcpserver.h>
#include <routes.h>
#include <headers.h>

#define MESSAGE_MAX_LEN 32384

typedef struct HTTP_Server{
    unsigned short port;
    struct hashmap *routes;
    TCP_Server *_tcp_server;
} HTTP_Server;

struct thread_arg
{
    HTTP_Server *server;
    int clientfd;
};


HTTP_Server* create_server(unsigned short port);

void handle_route(char* path, Request_Handler handler);

void handle_static(char* path);

void start();

#endif

