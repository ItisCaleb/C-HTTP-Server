#include <tcpserver.h>
#include <stdio.h>
#include <poll.h>

int iSetOption = 1;
struct timeval timeout = {.tv_sec = 1,.tv_usec=0};

struct TCP_Server* create_tcp_server(unsigned short port){
    TCP_Server *server = calloc(1,sizeof(struct TCP_Server));
    server->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server->sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption,sizeof(iSetOption));
    if(server->sockfd == -1){
        printf("Error create socket\n");
        exit(0);
    }
    server->addrlen = sizeof(struct sockaddr_in);

    //init server addr
    server->server_addr = calloc(1,server->addrlen);
    server->server_addr->sin_family = AF_INET;
    server->server_addr->sin_addr.s_addr = htonl(INADDR_ANY);
    server->server_addr->sin_port = htons(port);


    if(bind(server->sockfd, (struct sockaddr*)server->server_addr, server->addrlen) == -1){
        printf("Can't bind to port %d\n",port);
        exit(0);
    }
    server->client_addr = calloc(1, server->addrlen);
    return server;
}

void start_tcp(TCP_Server* server){
    if(!server){
        printf("Error: Your TCP server is NULL\n");
        exit(0);
    }
    listen(server->sockfd, 10);
}

int accept_tcp(TCP_Server* server){
    int clientfd = accept(server->sockfd, (struct sockaddr*)server->client_addr, &(server->addrlen));
    if(clientfd == -1){
        printf("Error: INVALID SOCKET\n");
    }
    return clientfd;
}

int recv_tcp(int fd, char* buffer, size_t size){
    struct pollfd p;
    p.fd = fd;
    p.events = POLLIN;
    int ret = poll(&p, 1, 5 * 1000);
    switch (ret)
    {
    case -1:
    case 0:
        return ret;   
    default:
        return recv(fd, buffer, size, 0);
    }
}


void send_tcp(int fd, char* payload, size_t size){
    send(fd,payload,size,0);
}