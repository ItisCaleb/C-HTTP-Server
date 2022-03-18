#ifndef TCP_SERVER
#define TCP_SERVER

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>

typedef struct TCP_Server{
    int sockfd;
    struct sockaddr_in *server_addr;
    struct sockaddr_in *client_addr;
    uint addrlen;
} TCP_Server;

struct TCP_Server* create_tcp_server(unsigned short port);

void start_tcp(TCP_Server* server);

int accept_tcp(TCP_Server* server);

int recv_tcp(int fd, char* buffer, size_t size);

void send_tcp(int fd, char* payload, size_t size);


#endif