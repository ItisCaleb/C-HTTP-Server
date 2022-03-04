
#ifndef REPONSE_H
#define REPONSE_H
#include <utils/hashmap.h>

typedef enum HTTP_Status{
    OK,
    Not_Found,
    Bad_Request,
    Not_Implemented,
    HTTP_Version_Not_Supported,
} HTTP_Status;

extern const int status_code[];

extern const char* status[]; 

typedef struct HTTP_Response{
    HTTP_Status status;
    float HTTP_VERSION;
    struct hashmap* headers;
    char* data;
    size_t data_len;
} HTTP_Response;

HTTP_Response* create_response();

void res_set_status(HTTP_Response* res, HTTP_Status status);

void res_set_data(HTTP_Response* res, char* data);

void res_set_html(HTTP_Response* res, char* file_path);

void concat_headers(HTTP_Response* res,char* header_str);

void concat_data(HTTP_Response* res,char* header_str);

void free_response(HTTP_Response* res);

#endif