#include <httpserver.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


struct HTTP_Server *create_server(unsigned short port)
{
  HTTP_Server *server = calloc(1, sizeof(HTTP_Server));
  server->port = port;
  server->_tcp_server = create_tcp_server(port);
  server->routes = create_router();
  return server;
}

void handle_route(HTTP_Server *server, char *path, Request_Handler handler)
{
  if (!server)
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
  add_route(server->routes, path, handler);
}

static void handle_response(HTTP_Server *server,HTTP_Request *req, HTTP_Response *res,int clientfd)
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
  struct thread_arg* arg = (struct thread_arg*)p;
  char *buffer = calloc(1,MESSAGE_MAX_LEN);
  while (recv_tcp(arg->clientfd, buffer, MESSAGE_MAX_LEN)){
     // do something
    HTTP_Request *req = create_request(buffer);
    HTTP_Response *res = create_response();
    if(!req->_parse_ok){
      res_set_status(res,req->_status);
    }
    if(req->_parse_ok && !execute_handler(arg->server->routes,req->path,req,res)){
      res_set_status(res,Not_Found);
    }
    handle_response(arg->server,req, res,arg->clientfd);
    bzero(buffer, MESSAGE_MAX_LEN);
    fflush(stdout);
  }
  shutdown(arg->clientfd, SHUT_WR);
  shutdown(arg->clientfd, SHUT_RD);
  close(arg->clientfd);
  free(buffer);
  free(arg);
  return NULL;
}

static void run_http(void *s){
  HTTP_Server *server = s;
  for (;;)
  {
    int clientfd =  accept_tcp(server->_tcp_server);
    if (clientfd != -1)
    {
      pthread_t t; // 宣告 pthread 變數
      struct thread_arg *arg = calloc(1,sizeof(struct thread_arg));
      arg->clientfd = clientfd;
      arg->server = server;
      pthread_create(&t, NULL, handle_request, arg);
    }
  }
}

void start(HTTP_Server *server)
{
  if (!server)
  {
    printf("Error: Your server is NULL\n");
    exit(0);
  }
  start_tcp(server->_tcp_server);
  run_http(server);
}

