#include <path.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

bool normalize(char* path){
    if(!http_decode(path)) {
        return false;
    }
    //printf("%s\n",path);

    return true;
}

bool http_decode(char* path){
    size_t len = strlen(path);
    char* tmp = calloc(1,len);
    int p=0;
    char hex[2];
    for(int i=0;i<len;i++){
        if(path[i] != '%'){
            tmp[p] = path[i];
            p++;
        }else if(i+1 < len && i+2 < len){
            hex[0] = path[i+1];
            hex[1] = path[i+2];
            tmp[p] = (char)strtol(hex, NULL, 16);
            if(tmp[p] == 0){
                return false;
            }
            i+=2;
            p++;
        }else{
            return false;
        }
    }
    memset(path, 0, len);
    strncpy(path, tmp, strlen(tmp));
    return true;
}

