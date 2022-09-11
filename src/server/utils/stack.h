#ifndef STACK_H
#define STACK_H
#include <stdbool.h>

typedef struct stack stack;

stack* stack_new();

bool is_stack_empty(stack* stack);

char* stack_pop(stack* stack);

void stack_push(stack* stack, char* data);

void stack_concat(stack* stack, char* str);

void stack_free(stack* stack);


#endif