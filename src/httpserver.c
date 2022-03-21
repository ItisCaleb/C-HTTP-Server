#include <httpserver.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

HTTP_Server *SERVER;

struct HTTP_Server *create_server(unsigned short port)
{
  HTTP_Server *server = calloc(1, sizeof(HTTP_Server));
  server->port = port;
  server->_tcp_server = create_tcp_server(port);
  server->routes = create_router();
  SERVER = server;
  return server;
}

void handle_route(char *path, Request_Handler handler)
{
  if (!SERVER)
  {
    printf("Error: Your server is NULL\n");
    exit(0);
  }
  if (!path)
  {
    printf("Error: Your path is NULL\n");
    exit(0);
  }
  if (!handler)
  {
    printf("Error: Your handler is NULL\n");
    exit(0);
  }
  add_route(SERVER->routes, path, handler);
}

static void handle_response(HTTP_Request *req, HTTP_Response *res,int clientfd)
{
  char *response = calloc(1, HEADER_MAX_LEN + res->data_len + 1);
  concat_headers(res, response);
  concat_data(res, response);

  // send response
  send_tcp(clientfd, response, strlen(response));

  // free response
  free(response);
  free_response(res);

  //free request
  free_request(req);
}

static void* handle_request(void* p){
  int clientfd = *(int*)p;
  char *buffer = calloc(1,MESSAGE_MAX_LEN);
  while (recv_tcp(clientfd, buffer, MESSAGE_MAX_LEN)){
     // do something
    HTTP_Request *req = create_request(buffer);
    HTTP_Response *res = create_response();
    if(!req->_parse_ok){
      res_set_status(res,req->_status);
    }
    if(req->_parse_ok && !execute_handler(SERVER->routes,req->path,req,res)){
      res_set_status(res,Not_Found);
    }
    handle_response(req, res, clientfd);
    bzero(buffer, MESSAGE_MAX_LEN);
    fflush(stdout);
  }

  shutdown(clientfd, SHUT_WR);
  shutdown(clientfd, SHUT_RD);
  close(clientfd);
  free(buffer);
  free(p);
  return NULL;
}

static void run_http(){
  HTTP_Server *server = SERVER;
  for (;;)
  {
    int clientfd =  accept_tcp(server->_tcp_server);
    if (clientfd != -1)
    {
      pthread_t t; // 宣告 pthread 變數
      int *fd = malloc(sizeof(int));
      *fd = clientfd;
      pthread_create(&t, NULL, handle_request, fd);
    }
  }
}

void stop_server(int sig){
  free(SERVER->_tcp_server->server_addr);
  free(SERVER->_tcp_server->client_addr);
  free(SERVER->_tcp_server);
  hashmap_free(SERVER->routes);
  exit(0);
}

void start()
{
  signal(SIGINT, stop_server);
  if (!SERVER)
  {
    printf("Error: Your server is NULL\n");
    exit(0);
  }
  start_tcp(SERVER->_tcp_server);
  run_http();
}

