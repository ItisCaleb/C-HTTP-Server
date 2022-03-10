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
  add_route(server->routes, path, handler);
}

static void handle_response(HTTP_Server *server,HTTP_Request *req, HTTP_Response *res)
{
  char *response = calloc(1, HEADER_MAX_LEN + res->data_len + 1);
  concat_headers(res, response);
  concat_data(res, response);

  // send response
  send_tcp(server->_tcp_server, response, strlen(response));

  // free response
  free(response);
  free_response(res);

  //free request
  free_request(req);
}

static void* run_http(void *s){
  HTTP_Server *server = s;
  for (;;)
  {
    if (accept_tcp(server->_tcp_server))
    {
      char buffer[8092] = {};
      if (recv_tcp(server->_tcp_server, buffer, sizeof(buffer)))
      {
        // do something
        HTTP_Request *req = create_request(buffer);
        HTTP_Response *res = create_response();
        if(!req->_parse_ok){
          res_set_status(res,req->_status);
        }
        if(req->_parse_ok && !execute_handler(server->routes,req->path,req,res)){
          res_set_status(res,Not_Found);
        }
        fflush(stdout);
        handle_response(server,req, res);
      }
      shutdown(server->_tcp_server->clientfd, SHUT_RD);
      close(server->_tcp_server->clientfd);
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
  pthread_t t[4]; // 宣告 pthread 變數
  for(int i=0;i<4;i++){
    pthread_create(&t[i], NULL, run_http, server);
  }
  pthread_join(t[0], NULL);
}

