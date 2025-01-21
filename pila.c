#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct node{
    int val;
    struct node* next;
}Node;

void push(int val, Node** head){
    Node* new_node = (Node*) malloc(sizeof(Node));
    new_node->val = val;
    new_node->next = *head;
    *head = new_node;
}

int pop(Node** head){
    if(*head == NULL){
        return NULL;
    }

    Node* to_delete = *head;
    *head = (*head)->next;
    int val = to_delete->val;
    free(to_delete);
    return val;
}