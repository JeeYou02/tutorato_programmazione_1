#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAMESIZE 255

#define min(x,y) (x < y)? x : y

typedef struct{
    char* in_filename;
    char* out_filename;
}Parameters;

typedef struct node{
    char nome[NAMESIZE];
    char cognome[NAMESIZE];
    int numero_conto;
    int anno_apertura;
    float saldo;
    struct node* next;
}Node;

typedef struct{
    Node* head;
    int length;
}List;

void error(char* err){
    fprintf(stderr, "[ERROR] %s", err);
    exit(1);
}

Node* createNode(char* nome, char* cognome, int numero_conto, int anno_apertura, float saldo){
    Node* n = (Node*) malloc(sizeof(Node));

    strcpy(n->nome, nome);
    strcpy(n->cognome, cognome);
    n->numero_conto = numero_conto;
    n->anno_apertura = anno_apertura;
    n->saldo = saldo;
    n->next = NULL;

    return n;
}

void insertNode(Node* new_node, List* l){
    if(l->length == 0){         //la lista è vuota
        l->head = new_node;
        l->length++;
        new_node->next = NULL;
        return;
    }

    Node* curr = l->head;
    Node* next = curr->next;

    if(new_node->anno_apertura < l->head->anno_apertura){       //il nodo va inserito prima della head
        new_node->next = l->head;
        l->head = new_node;
        l->length++;
        return;
    }

    while(next != NULL && next->anno_apertura < new_node->anno_apertura){       //ricerca della posizione di inserimento
        curr = next;
        next = curr->next;
    }

    curr->next = new_node;      //inserimento
    new_node->next = next;
    l->length++;
}

void deleteNode(List* l, int numero_conto){
    printf("[DEBUG] Deleting account number: %d\n", numero_conto);
    if(l->head->numero_conto == numero_conto){      //il nodo da eliminare è in testa
        Node* to_delete = l->head;
        l->head = to_delete->next;
        printf("[DEBUG] Account %d removed...\n", to_delete->numero_conto);
        free(to_delete);
        l->length--;
        return;
    }

    Node* prev = l->head;
    Node* curr = l->head->next;
    while(curr){
        if(curr->numero_conto == numero_conto){
            prev->next = curr->next;
            printf("[DEBUG] Account %d removed...\n", curr->numero_conto);
            free(curr);
            l->length--;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void printList(List* l){
    printf("===========PRINT LIST===========\n");

    if(l->head == NULL){
        printf("The list is empty...\n");
        return;
    }

    Node* curr = l->head;
    while(curr != NULL){
        printf("%s %s %d %d %f\n", curr->nome, curr->cognome, curr->numero_conto, curr->anno_apertura, curr->saldo);
        curr = curr->next;
    }
    printf("================================\n");
}

Parameters readInput(int argc, char* argv[]){
    if(argc != 3)
        error("Invalid number of parameters...\n");

    Parameters p;
    p.in_filename = argv[1];
    p.out_filename = argv[2];

    if(strcmp(p.in_filename + strlen(p.in_filename) - 4, ".txt") || strcmp(p.out_filename + strlen(p.out_filename) - 4, ".txt"))
        error("Both files must be of type \"filename.txt\"...\n");

    return p;
}

List* readFile(char* filename){
    FILE* f = fopen(filename, "r");
    List* l = (List*) malloc(sizeof(List));
    l->head = NULL;
    l->length = 0;
    char nome[NAMESIZE];
    char cognome[NAMESIZE];
    int numero_conto;
    int anno_apertura;
    float saldo;

    while(!feof(f)){
        fscanf(f, "%s %s %d %d %f", nome, cognome, &numero_conto, &anno_apertura, &saldo);
        if(!feof(f)){
            Node* n = createNode(nome, cognome, numero_conto, anno_apertura, saldo);
            insertNode(n, l);
        }
    }

    fclose(f);
    return l;
}

float getMax(List* l){
    float max = l->head->saldo;

    Node* curr = l->head->next;
    while(curr != NULL){
        if(curr->saldo > max)
            max = curr->saldo;
        
        curr = curr->next;
    }
    
    return max;
}

void removeAccount(List* l){
    float m = getMax(l);

    Node* curr = l->head->next;
    float max_x = min((2023-l->head->anno_apertura)/5, 1) * l->head->saldo/m;
    int max_conto = l->head->numero_conto;

    while(curr){
        float x = min((2023-curr->anno_apertura)/5, 1) * curr->saldo/m;
        
        if(x > max_x){
            max_x = x;
            max_conto = curr->numero_conto;
        }

        curr = curr->next;
    }

    deleteNode(l, max_conto);
}

void writeFile(char* filename, List* l){
    FILE* f = fopen(filename, "w");

    Node* curr = l->head;
    while(curr){
        fprintf(f, "%s %s %d %d %f\n", curr->nome, curr->cognome, curr->numero_conto, curr->anno_apertura, curr->saldo);
        curr = curr->next;
    }

    fclose(f);
}

int main(int argc, char* argv[]){
    Parameters p = readInput(argc,argv);
    printf("[DEBUG] Input read...\n");
    List* l = readFile(p.in_filename);
    printf("[DEBUG] List created...\n");
    printf("[DEBUG] Head = %p\n", l->head);
    printList(l);
    int h = l->length/2;
    for(int i = 0; i < h; i++){
        removeAccount(l);
        printf("[DEBUG] Head = %p\n", l->head);
    }

    writeFile(p.out_filename, l);
    printf("[DEBUG] File written...\n");
}