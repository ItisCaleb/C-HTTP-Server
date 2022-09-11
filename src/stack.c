#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils/stack.h"
#include "utils/list.h"

typedef struct stack{
    list_node* list;
} stack;

#define panic(_msg_)                                                           \
{                                                                              \
    fprintf(stderr, "panic: %s (%s:%d)\n", (_msg_), __FILE__, __LINE__);       \
    exit(1);                                                                   \
}

stack* stack_new(){
    stack* stack = calloc(1,sizeof(stack));
    stack->list = list_new();
    return stack;
}

bool is_stack_empty(stack* stack){
    if(stack == NULL){
        panic("Stack is NULL");
        return true;
    }
    return is_list_empty(stack->list);
}

char* stack_pop(stack* stack){
    if(is_stack_empty(stack)){
        panic("Stack is NULL or empty");
        return NULL;
    }
    char* data = stack->list->prev->value;
    list_remove_tail(stack->list);
    return data;
}

void stack_push(stack* stack, char* data){
    if(stack == NULL){
        panic("Stack is NULL");
        return;
    }
    list_node* node = list_new();
    node->value = malloc(strlen(data)+1);
    strcpy(node->value, data);
    list_add_tail(stack->list, node);
}

void stack_concat(stack* stack, char* str){
    if(is_stack_empty(stack)){
        str[0] = '/';
        return;
    }
    list_node* cur;
    list_for_each(cur, stack->list){
        char* tmp = cur->value;
        strcat(str,"/");
        if(tmp != NULL){
            strcat(str,tmp);
        }
    }
}

void stack_free(stack* stack){
    list_free(stack->list);
}
