#ifndef HTTP_SERVER
#define HTTP_SERVER

#include <routes.h>
#include <headers.h>

#define MESSAGE_MAX_LEN 32384

typedef struct HTTP_Server HTTP_Server;

void create_server(unsigned short port);

void handle_route(char* path, Request_Handler handler);

void handle_static(char* path);

void start();

#endif

