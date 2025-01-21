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

void deleteNext(Node* prec, List* l){       //se il primo parametro è NULL, cancella la testa della lista, altrimenti cancella il successore del primo parametro
    if(prec == NULL){       //dobbiamo cancellare la testa della lista
        Node* to_delete = l->head;
        l->head = to_delete->next;
        free(to_delete);
        l->length--;
        return;
    }

    Node* to_delete = prec->next;       //cancelliamo il successore di prec
    printf("[DEBUG] Removing account %s %s...\n", to_delete->nome, to_delete->cognome);
    prec->next = to_delete->next;
    free(to_delete);
    l->length--;
    return;
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

    Node* curr = l->head;
    Node* next = curr->next;

    float max_x = min((2023 - l->head->anno_apertura)/5.0, 1) * (l->head->saldo / m);
    Node* max_prec = NULL;

    while(next != NULL){
        float x = min((2023 - next->anno_apertura)/5.0, 1) * (next->saldo / m);
        
        if(x > max_x){
            max_x = x;
            max_prec = curr;
        }
        curr = next;
        next = curr->next;
    }
    printf("\n");
    deleteNext(max_prec, l);
}

void writeFile(char* filename, List* l){
    FILE* f = fopen(filename, "w");
    
    Node* curr = l->head;

    while(curr != NULL){
        fprintf(f, "%s %s %d %d %f\n", curr->nome, curr->cognome, curr->numero_conto, curr->anno_apertura, curr->saldo);
        curr = curr->next;
    }

    fclose(f);
}

int main(int argc, char* argv[]){
    Parameters p = readInput(argc, argv);
    List* l = readFile(p.in_filename);
    printList(l);
    printf("\n");
    int h = l->length/2;
    for(int i = 0; i < h; i++){
        if(i == h-1) printList(l);
        removeAccount(l);
    }
    printf("\n");
    printList(l);
    writeFile(p.out_filename, l);
}
