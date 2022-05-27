
#ifndef REPONSE_H
#define REPONSE_H
#include <utils/hashmap.h>

typedef enum HTTP_Status{
    Continue,
    Switching_Protocol,
    OK,
    Created,
    Accepted,
    Non_Authoritative_Information,
    No_Content,
    Reset_Content,
    Multiple_Choices,
    Moved_Permanently,
    Found,
    See_Other,
    Not_Modified,
    Use_Proxy,
    Temporary_Redirect,
    Bad_Request,
    Payment_Required,
    Forbidden,
    Not_Found,
    Method_Not_Allowed,
    Not_Acceptable,
    Request_Timeout,
    Conflict,
    Gone,
    Lenght_Required,
    Payload_Too_Large,
    URI_Too_Long,
    Unsupported_Media_Type,
    Expectation_Failed,
    Upgrade_Required,
    Internal_Server_Error,
    Not_Implemented,
    Bad_Gateway,
    Service_Unavailable,
    Gateway_Timeout,
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

void res_set_file(HTTP_Response* res, char* file_path, char* type);

void res_redirect(HTTP_Response* res, char* path);

void concat_headers(HTTP_Response* res,char* header_str);

void concat_data(HTTP_Response* res,char* header_str);

void free_response(HTTP_Response* res);

#endif