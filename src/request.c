#include <request.h>
#include <stdlib.h>
#include <stdio.h>
#include <headers.h>
#include <string.h>

const char* constants[] = {methods};

HTTP_method str2enum(char* name) {
    int i;
    for (i = 0; i < sizeof(constants) / sizeof(constants[0]); ++i) {
        if (!strcmp(name, constants[i])) {
            return (HTTP_method)i;
        }   
    }   
    return (HTTP_method)-1;
}   

bool parse_first_line(HTTP_Request *req, char* buffer){
  //get firstline
  int len = strcspn(buffer,"\r\n");
  if(len >= REQUEST_LINE_MAX_LEN){
    req->_status = Bad_Request;
    return false;
  }
  char *sline = calloc(1, REQUEST_LINE_MAX_LEN);
  strncpy(sline,buffer,len);

  //get method
  char *p = strtok(sline, " ");
  req->method = str2enum(p);
  if(req->method == -1){
    free(sline);
    req->_status = Not_Implemented;
    return false;
  }

  //get path
  p = strtok(NULL, " ");
  if(p == NULL || p[0] != '/'){
    free(sline);
    req->_status = Bad_Request;
    return false;
  }
  strcpy(req->path, p);

  //get version
  p = strtok(NULL, " ");
  if(p == NULL || strncmp(p,"HTTP/",5) != 0){
    free(sline);
    req->_status = Bad_Request;
    return false;
  }
  char ver[3] = {};
  strncpy(ver,p+5,3);
  req->HTTP_VERSION = atof(ver);
  if(req->HTTP_VERSION <= 0.8 || req->HTTP_VERSION >= 2.0){
    free(sline);
    req->_status = HTTP_Version_Not_Supported;
    return false;
  }
  free(sline);
  return true;
}

bool parse_headers(HTTP_Request *req, char* buffer){
  
  return true;
}

bool parse_request(HTTP_Request *req, char* buffer){
  if(!parse_first_line(req,buffer)){
    return false;
  }
  return true;
}

HTTP_Request* create_request(char* buffer) {
  HTTP_Request *req = calloc(1, sizeof(HTTP_Request));
  req->path = calloc(1, REQUEST_LINE_MAX_LEN);
  req->_parse_ok = parse_request(req,buffer);
  return req;
}

