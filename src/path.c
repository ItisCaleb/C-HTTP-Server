#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "path.h"
#include "utils/stack.h"

char* normalize(char* path){
    char *dpath = http_decode(path);
    if(dpath == NULL) {
        return NULL;
    }
    dpath = remove_dot(dpath);
    return dpath;
}

char* http_decode(char* path){
    size_t len = strlen(path);
    char* decoded = calloc(1,len);
    int p=0;
    char hex[2];
    for(int i=0;i<len;i++){
        if(path[i] != '%'){
            decoded[p] = path[i];
            p++;
        }else if(i+1 < len && i+2 < len){
            hex[0] = path[i+1];
            hex[1] = path[i+2];
            decoded[p] = (char)strtol(hex, NULL, 16);
            if(decoded[p] == 0){
                free(decoded);
                return NULL;
            }
            i+=2;
            p++;
        }else{
            free(decoded);
            return NULL;
        }
    }
    return decoded;
}

char* remove_dot(char* path){
    int size = strlen(path)+1;
    char* tmp_p = NULL;
    char* seg = strtok_r(path, "/", &tmp_p);
    stack* st = stack_new();
    while (seg!=NULL)
    {   
        if(strcmp(seg,"..") == 0){
            if(!is_stack_empty(st)){
                stack_pop(st);
            }
        }else if(strcmp(seg,".")!=0) stack_push(st,seg);
        seg = strtok_r(NULL, "/", &tmp_p);
    }
    char* new_path = calloc(1,size);
    stack_concat(st, new_path);
    free(path);
    stack_free(st);
    return new_path;
}