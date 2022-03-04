#include <headers.h>
#include <response.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const int status_code[] = {
  [OK] = 200, 
  [Not_Found] = 404, 
  [Bad_Request] = 400, 
  [Not_Implemented] = 501,
  [HTTP_Version_Not_Supported] = 505
};

const char *status[] = {
    [OK] = "OK", 
    [Not_Found] = "Not Found", 
    [Bad_Request] = "Bad Request",
    [Not_Implemented] = "Not Implemented",
    [HTTP_Version_Not_Supported] = "HTTP Version Not Supported"
};

char* get_time_str() {
  time_t t = time(NULL);
  struct tm *tm = gmtime(&t);
  char *date_str = calloc(1, 40);
  strftime(date_str, 40, "%a, %d %b %Y %X GMT", tm);
  return date_str;
}

void res_set_status(HTTP_Response *res, HTTP_Status status) {
  res->status = status;
}

void res_set_data(HTTP_Response *res, char *data) {
  res->data = data;
  res->data_len = strlen(data);
}

void res_set_html(HTTP_Response* res, char* file_path){
  FILE *ptr = fopen(file_path,"r");
  if(!ptr){
    printf("Can't open file at %s\n",file_path);
    return;
  }
  fseek(ptr, 0, SEEK_END);
  long fsize = ftell(ptr);
  fseek(ptr, 0, SEEK_SET);  /* same as rewind(f); */

  char *data = malloc(fsize + 1);
  fread(data, fsize, 1, ptr);
  fclose(ptr);
  res_set_data(res,data);
}


HTTP_Response *create_response() {
  HTTP_Response *res = calloc(1, sizeof(HTTP_Response));
  res->HTTP_VERSION = 1.1;
  res->headers = create_headers();
  set_header(res->headers, "Server", "ItisCaleb/1.0");
  set_header(res->headers, "Date", get_time_str());
  set_header(res->headers, "Connection", "close");
  return res;
}

bool header_iter(const void *item, void *udata) {
  const HTTP_Header *header = item;
  char *field = calloc(1, strlen(header->name) + strlen(header->value) + 5);
  sprintf(field, "%s: %s\r\n", header->name, header->value);
  strcat((char *)udata, field);
  free(field);
  return true;
}

void concat_headers(HTTP_Response *res, char *header_str) {
  // first line
  sprintf(header_str, "HTTP/%3.1f %d %s\r\n", res->HTTP_VERSION,
          status_code[res->status], status[res->status]);

  if (status_code[res->status] >= 200 && status_code[res->status] != 204) {
    // add Content-Length
    char len[30] = {};
    sprintf(len, "%zu", res->data_len);
    set_header(res->headers, "Content-Length", len);
  }

  // append headers
  hashmap_scan(res->headers, header_iter, header_str);
  strcat(header_str, "\r\n");
  return;
}

void concat_data(HTTP_Response *res, char *header_str) {
  if (status_code[res->status] >= 200 && status_code[res->status] != 204) {
    strncat(header_str, res->data, res->data_len);
  }
  return;
}

void free_response(HTTP_Response *res) {
  hashmap_free(res->headers);
  if(res->data != NULL){
    free(res->data);
  }
  free(res);
}