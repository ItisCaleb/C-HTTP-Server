#include <headers.h>
#include <parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *constants[] = {methods};

HTTP_method str2enum(char *name) {
  int i;
  for (i = 0; i < sizeof(constants) / sizeof(constants[0]); ++i) {
    if (!strcmp(name, constants[i])) {
      return (HTTP_method)i;
    }
  }
  return (HTTP_method)-1;
}

int check_ending(char *buffer) {
  int len = strcspn(buffer, "\r\n");
  if (len < REQUEST_LINE_MAX_LEN) {
    if (buffer[len] == '\r') {
      if (buffer[len + 1] == '\n')
        return len;
    } else if (buffer[len] == '\n')
      return len;
  }
  return -1;
}

bool parse_first_line(HTTP_Request *req, char *buffer) {
  // get firstline
  int len = check_ending(buffer);
  if (len == -1) {
    req->_status = Bad_Request;
    return false;
  }
  char *sline = calloc(1, REQUEST_LINE_MAX_LEN);
  strncpy(sline, buffer, len);

  // get method
  char *p = strtok(sline, " ");
  req->method = str2enum(p);
  if (req->method == -1) {
    free(sline);
    req->_status = Not_Implemented;
    return false;
  }

  // get path
  p = strtok(NULL, " ");
  if (p == NULL || p[0] != '/') {
    free(sline);
    req->_status = Bad_Request;
    return false;
  }
  strcpy(req->path, p);

  // get version
  p = strtok(NULL, " ");
  if (p == NULL || strncmp(p, "HTTP/", 5) != 0) {
    free(sline);
    req->_status = Bad_Request;
    return false;
  }
  char ver[3] = {};
  strncpy(ver, p + 5, 3);
  req->HTTP_VERSION = atof(ver);
  if (req->HTTP_VERSION <= 0.8 || req->HTTP_VERSION >= 2.0) {
    free(sline);
    req->_status = HTTP_Version_Not_Supported;
    return false;
  }
  free(sline);
  return true;
}

bool parse_headers(HTTP_Request *req, char *buffer, ParseStatus *s) {
  int len = check_ending(buffer);
  if (len == 0) {
    *s = PARSE_DONE;
    return true;
  }
  if (len == -1)
    return false;
  char *line = calloc(1, REQUEST_LINE_MAX_LEN);
  strncpy(line, buffer, len);
  char *name = NULL;
  char *value = NULL;
  for (int i = 0; i < len; i++) {
    if (line[i] == ':') {
      line[i] = '\0';
      name = malloc(strlen(line) + 1);
      strncpy(name, line, strlen(line) + 1);
      value = malloc(strlen(&line[i + 1]) + 1);
      strncpy(value, &line[i + 1], strlen(&line[i + 1]) + 1);
      break;
    } else if (line[i] == ' ') {
      free(line);
      return false;
    }
  }
  free(line);
  if (name == NULL || value == NULL) {
    return false;
  }
  set_header(req->headers, name, value);
  free(name);
  free(value);
  return true;
}

bool parse_request(HTTP_Request *req, char *buffer) {
  char *ptr = buffer;
  if (!parse_first_line(req, ptr)) {
    return false;
  }
  ParseStatus s = PARSE_OK;
  do {
    ptr = strpbrk(ptr, "\r\n");
    if (ptr[0] == '\r' && ptr[1] == '\n')
      ptr += 2;
    else if (ptr[0] == '\n')
      ptr += 1;
    else
      s = PARSE_FALSE;
    if (s != PARSE_FALSE && !parse_headers(req, ptr, &s)) {
      s = PARSE_FALSE;
    }
  } while (s == PARSE_OK);
  if (ptr[0] == '\r' && ptr[1] == '\n')
    ptr += 2;
  else if (ptr[0] == '\n')
    ptr += 1;
  if (s == PARSE_FALSE) {
    req->_status = Bad_Request;
    return false;
  }
  HTTP_Header *cl = get_header(req->headers, "Content-Length");
  if (cl) {
    int l = atoi(cl->value);
    req->data = calloc(1, l + 1);
    strncpy(req->data, ptr, l);
  }
  return true;
}