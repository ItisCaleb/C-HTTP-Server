#ifndef HEADERS_H
#define HEADERS_H
#include <utils/hashmap.h>

#define HEADER_MAX_LEN 8092

typedef struct HTTP_Header
{
    char* name;
    char* value;
} HTTP_Header;

struct hashmap* create_headers();

void set_header(struct hashmap* map,char* name,char* value);

HTTP_Header* get_header(struct hashmap* map,char* name);

#endif