#ifndef PATH_H
#define PATH_H

#include <stdbool.h>

bool normalize(char* path);

bool http_decode(char* path);

bool remove_dot(char* path);

#endif