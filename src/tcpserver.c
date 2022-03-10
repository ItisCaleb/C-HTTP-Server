#include <tcpserver.h>
#include <stdio.h>

int iSetOption = 1;

struct TCP_Server* create_tcp_server(unsigned short port){
    TCP_Server *server = calloc(1,sizeof(struct TCP_Server));
    server->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server->sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption,sizeof(iSetOption));
    if(server->sockfd == -1){
        printf("Error create socket\n");
        exit(0);
    }
   
    //init server addr
    server->server_addr = calloc(1,server->addrlen);
    server->server_addr->sin_family = AF_INET;
    server->server_addr->sin_addr.s_addr = htonl(INADDR_ANY);
    server->server_addr->sin_port = htons(port);

    server->addrlen = sizeof(struct sockaddr_in);

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
    listen(server->sockfd, 5);
}

int accept_tcp(TCP_Server* server){
    server->clientfd = accept(server->sockfd, (struct sockaddr*)server->client_addr, &(server->addrlen));
    if(server->clientfd == -1){
        printf("Error: INVALID SOCKET\n");
        return 0;
    }
    return 1;
}

int recv_tcp(TCP_Server* server, char* buffer, size_t size){
    if(recv(server->clientfd, buffer, size, 0)==0){
        return 0;
    }
    return 1;
}


void send_tcp(TCP_Server* server, char* payload, size_t size){
    send(server->clientfd,payload,size,0);
}