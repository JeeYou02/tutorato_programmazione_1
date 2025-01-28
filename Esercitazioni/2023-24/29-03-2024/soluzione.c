#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFSIZE 16

typedef struct{
    char* filename;
}Parameters;

typedef struct node{
    int val;
    struct node* next;
}Node;

void error(char* err){
    fprintf(stderr, "[ERROR] %s", err);
    exit(1);
}

void push(int val, Node** P){
    Node* new_node = (Node*) malloc(sizeof(Node));
    new_node->val = val;
    new_node->next = *P;
    *P = new_node;
}

int pop(Node** P){
    if(*P == NULL)
        return -1;
    
    Node* to_delete = *P;
    *P = to_delete->next;
    int n = to_delete->val;
    free(to_delete);
    return n;
}

void printStack(Node* P){
    Node* curr = P;
    while(curr != NULL){
        printf("%d -> ", curr->val);
        curr = curr->next;
    }
    printf("NULL\n");
}

int stackSize(Node* P){
    int count = 0;
    Node* curr = P;
    while(curr != NULL){
        count++;
        curr = curr->next;
    }
    return count;
}

Parameters decodeParameters(int argc, char* argv[]){
    if(argc != 2)
        error("numero di parametri errato\n");
    
    if(strlen(argv[1]) > 100)
        error("nome del file troppo lungo\n");
    
    Parameters p;
    p.filename = argv[1];
    if(strcmp(p.filename + strlen(p.filename) - 4, ".txt") != 0)
        error("l'estensione del file non è corretta\n");
    
    return p;
}

Node* readFile(FILE* f){
    Node* P = NULL;
    int n;
    char line[BUFFSIZE];
    while(fgets(line, BUFFSIZE, f)){
        n = atoi(line);
        push(n, &P);
    }
    return P;
}

float getMean(Node** P){
    float m = 0;
    int count = 0;
    int n = pop(P);
    while(n != -1){
        m += n;
        count++;
        n = pop(P);
    }
    if(count > 0){
        m /= count;
        return m;
    } else {
        return -1;
    }
}

void fillP(FILE* f, Node** P, float m){
    int x;
    char line[BUFFSIZE];
    rewind(f);
    fgets(line, BUFFSIZE, f);
    push(atoi(line), P);

    while(fgets(line, BUFFSIZE, f)){
        x = atoi(line);
        if(x > m){
            push(x, P);
        } else {
            int y = pop(P);
            push((x+y)/2, P);
        }
    }
}

int* transferP(Node** P){
    int dim = stackSize(*P);
    int* A = (int*) malloc(sizeof(int) * dim);
    for(int i = 0; i < dim; i++){
        A[i] = pop(P);
    }
    return A;
}

void bubbleSort(int* A, int n){
    int aux;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n-i-1; j++){
            if(A[j] > A[j+1]){
                aux = A[j+1];
                A[j+1] = A[j];
                A[j] = aux;
            }
        }
    }
}

int main(int argc, char* argv[]){
    printf("==================PUNTO A==================\n");
    Parameters p = decodeParameters(argc, argv);
    printf("filename = %s\n\n", p.filename);

    printf("==================PUNTO B==================\n");
    FILE* f = fopen(p.filename, "r");
    Node* P = readFile(f);
    printStack(P);

    printf("==================PUNTO C==================\n");
    float m = getMean(&P);
    printf("mean = %f", m);

    
    //INCOMPLETO


    fclose(f);
}