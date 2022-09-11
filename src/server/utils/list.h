#ifndef LIST_H
#define LIST_H
#include <stdbool.h>

#define list_for_each(cur,head) \
    for(cur = (head)->next;cur != (head);cur=cur->next)

typedef struct list_node{
    struct list_node* next;
    struct list_node* prev;
    char* value;
} list_node;

list_node* list_new();

void list_add_head(list_node* head, list_node* new_node);

void list_add_tail(list_node* head, list_node* new_node);

void list_remove_head(list_node* head);

void list_remove_tail(list_node* head);

int list_size(list_node* head);

bool is_list_empty(list_node* head);

void node_free(list_node* node);

void list_free(list_node* head);

#endif