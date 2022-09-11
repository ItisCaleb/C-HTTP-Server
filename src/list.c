#include <stdlib.h>
#include "utils/list.h"

list_node* list_new(){
    list_node* head = calloc(1,sizeof(list_node));
    head->next = head;
    head->prev = head;
    return head;
}

void list_add_head(list_node* head, list_node* new_node){
    head->next->prev = new_node;
    new_node->next = head->next;
    head->next = new_node;
    new_node->prev = head;
}

void list_add_tail(list_node* head, list_node* new_node){
    head->prev->next = new_node;
    new_node->prev = head->prev;
    head->prev = new_node;
    new_node->next = head;
}

void list_remove_head(list_node* head){
    list_node* next = head->next;
    head->next = next->next;
    next->next->prev = head;
    node_free(next);
}

void list_remove_tail(list_node* head){
    list_node* prev = head->prev;
    head->prev = prev->prev;
    prev->prev->next = head;
    node_free(prev);
}

int list_size(list_node* head){
    list_node* cur;
    int len = 0;
    list_for_each(cur,head)
        len++;
    return len;
}

bool is_list_empty(list_node* head){
    return (head->next == head);
} 

void node_free(list_node* node){
    if(node->value == NULL){
        free(node->value);
    }
    free(node);
}

void list_free(list_node* head){
    list_node* cur;
    list_for_each(cur, head){
        node_free(cur);
    }
    node_free(head);
}