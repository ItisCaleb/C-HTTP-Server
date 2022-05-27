#include <headers.h>
#include <response.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const int status_code[] = {[Continue] = 100,
                           [Switching_Protocol] = 101,
                           [OK] = 200,
                           [Created] = 201,
                           [Accepted] = 202,
                           [Non_Authoritative_Information] = 203,
                           [No_Content] = 204,
                           [Reset_Content] = 205,
                           [Multiple_Choices] = 300,
                           [Moved_Permanently] = 301,
                           [Found] = 302,
                           [See_Other] = 303,
                           [Not_Modified] = 304,
                           [Use_Proxy] = 305,
                           [Temporary_Redirect] = 307,
                           [Bad_Request] = 400,
                           [Payment_Required] = 402,
                           [Forbidden] = 403,
                           [Not_Found] = 404,
                           [Method_Not_Allowed] = 405,
                           [Not_Acceptable] = 406,
                           [Request_Timeout] = 408,
                           [Conflict] = 409,
                           [Gone] = 410,
                           [Lenght_Required] = 411,
                           [Payload_Too_Large] = 413,
                           [URI_Too_Long] = 414,
                           [Unsupported_Media_Type] = 415,
                           [Expectation_Failed] = 417,
                           [Upgrade_Required] = 426,
                           [Internal_Server_Error] = 500,
                           [Not_Implemented] = 501,
                           [Bad_Gateway] = 502,
                           [Service_Unavailable] = 503,
                           [Gateway_Timeout] = 504,
                           [HTTP_Version_Not_Supported] = 505};

const char *status[] = {
    [Continue] = "Continue",
    [Switching_Protocol] = "Switching Protocol",
    [OK] = "OK",
    [Created] = "Created",
    [Accepted] = "Accepted",
    [Non_Authoritative_Information] = "Non-Authoritative Information",
    [No_Content] = "No Content",
    [Reset_Content] = "Reset Content",
    [Multiple_Choices] = "Multiple Choices",
    [Moved_Permanently] = "Moved Permanently",
    [Found] = "Found",
    [See_Other] = "See Other",
    [Not_Modified] = "Not Modified",
    [Use_Proxy] = "Use Proxy",
    [Temporary_Redirect] = "Temporary Redirect",
    [Bad_Request] = "Bad Request",
    [Payment_Required] = "Payment Required",
    [Forbidden] = "Forbidden",
    [Not_Found] = "Not Found",
    [Method_Not_Allowed] = "Method Not Allowed",
    [Not_Acceptable] = "Not Acceptable",
    [Request_Timeout] = "Request Timeout",
    [Conflict] = "Conflict",
    [Gone] = "Gone",
    [Lenght_Required] = "Lenght Required",
    [Payload_Too_Large] = "Payload Too Large",
    [URI_Too_Long] = "URI Too Long",
    [Unsupported_Media_Type] = "Unsupported Media Type",
    [Expectation_Failed] = "Expectation Failed",
    [Upgrade_Required] = "Upgrade Required",
    [Internal_Server_Error] = "Internal Server Error",
    [Not_Implemented] = "Not Implemented",
    [Bad_Gateway] = "Bad Gateway",
    [Service_Unavailable] = "Service Unavailable",
    [Gateway_Timeout] = "Gateway Timeout",
    [HTTP_Version_Not_Supported] = "HTTP Version Not Supported"};

char *get_time_str() {
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
  size_t len = strlen(data);
  char *cdata = malloc(len+1);
  strncpy(cdata, data, len+1);
  if(res->data != NULL){
    free(res->data);
  }
  res->data = cdata;
  res->data_len = len;
}

void res_redirect(HTTP_Response *res, char *path) {
  res_set_status(res, Found);
  set_header(res->headers, "Location", path);
}

void res_set_html(HTTP_Response *res, char *file_path) {
  FILE *ptr = fopen(file_path, "r");
  if (!ptr) {
    printf("Can't open file at %s\n", file_path);
    return;
  }
  set_header(res->headers, "Content-Type", "text/html; charset=UTF-8");
  fseek(ptr, 0, SEEK_END);
  long fsize = ftell(ptr);
  fseek(ptr, 0, SEEK_SET); /* same as rewind(f); */

  char *data = malloc(fsize + 1);
  fread(data, fsize, 1, ptr);
  fclose(ptr);
  res_set_data(res, data);
  free(data);
}

void res_set_file(HTTP_Response* res, char* file_path, char* type){
  FILE *ptr = fopen(file_path, "r");
  if (!ptr) {
    printf("Can't open file at %s\n", file_path);
    return;
  }
  set_header(res->headers, "Content-Type", "image/gif");
  fseek(ptr, 0, SEEK_END);
  long fsize = ftell(ptr);
  fseek(ptr, 0, SEEK_SET); /* same as rewind(f); */

  char *data = malloc(fsize + 1);
  fread(data, fsize, 1, ptr);
  fclose(ptr);
  res_set_data(res, data);
  free(data);
}

HTTP_Response *create_response() {
  HTTP_Response *res = calloc(1, sizeof(HTTP_Response));
  res->HTTP_VERSION = 1.1;
  res->headers = create_headers();
  res->status = OK;
  char *time_str = get_time_str();
  set_header(res->headers, "Server", "ItisCaleb/1.0");
  set_header(res->headers, "Date", time_str);
  set_header(res->headers, "Connection", "keep-alive");
  set_header(res->headers, "Content-Type", "text/plain");
  free(time_str);
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
  if (!res) {
    printf("You can't pass NULL response!");
    return;
  }
  hashmap_free(res->headers);
  if (res->data != NULL) {
    free(res->data);
  }
  free(res);
}