#ifndef PATH_H
#define PATH_H

#include <stdbool.h>

char* normalize(char* path);

char* http_decode(char* path);

char* remove_dot(char* path);

#endif