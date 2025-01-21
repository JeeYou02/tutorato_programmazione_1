#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define BUFFSIZE 256

typedef struct{
    char* in_filename;
    char* out_filename;
}Parameters;

typedef struct node{
    char* val;
    struct node* next;
}Node;

typedef struct{
    Node* head;
    int length;
}List;

void error(char* err){
    fprintf(stderr, "[ERROR] %s\n", err);
    exit(1);
}

char isVowel(char c){
    return tolower(c) == 'a' || tolower(c) == 'e' || tolower(c) == 'i' || tolower(c) == 'o' || tolower(c) == 'u';
}

int vowelCounter(char* s){
    int counter = 0;
    for(int i = 0; i < strlen(s); i++){
        if(isVowel(s[i]))
            counter++;
    }
    return counter;
}

void insertNode(List* l, char* val){
    Node* new_node = (Node*) malloc(sizeof(Node));
    new_node->val = (char*) malloc(sizeof(char) * strlen(val)); 
    strcpy(new_node->val, val);
    new_node->next = NULL;         //inizializzazione nuovo nodo

    if(l->length == 0){         //la lista è vuota
        l->head = new_node;
        l->length++;
        return;
    }

    Node* curr = l->head;
    Node* next = curr->next;

    if(strcmp(new_node->val, curr->val) <= 0){   //l'elemento va inserito prima della head
        l->head = new_node;
        new_node->next = curr;
        l->length++;
        return;
    }

    while(next != NULL){     //ricerca della posizione di inserimento
        if(strcmp(new_node->val,next->val) < 0){
            curr->next = new_node;
            new_node->next = next;      //inserimento del nodo
            l->length++;
            return;
        }
        curr = next;
        next = curr->next;
    }
    curr->next = new_node;  //l'elemento va inserito alla fine della lista
    l->length++;
}

void deleteNode(List* l, char* val){
    if(!strcmp(l->head->val, val) && l->head->next == NULL){    //il nodo è l'unico presente in lista
        free(l->head);
        l->head = NULL;
        l->length--;
        return;
    }

    if(!strcmp(l->head->val, val)){    //il nodo è in testa ma non è l'unico nodo della lista
        Node* next = l->head->next;
        free(l->head);
        l->head = next;
        l->length--;
        return;
    }

    Node* curr = l->head;
    Node* next = curr->next;
    while(strcmp(next->val, val) && next != NULL){   //ricerca del nodo
        curr = next;
        next = curr->next;
    }

    if(next == NULL)    //l'elemento non è in lista
        return;

    curr->next = next->next;    //elemento trovato in lista
    free(next);
    l->length--;
    return;
}

void printList(List* l){
    Node* curr = l->head;

    if(curr == NULL){
        printf("The list is empty...\n");
        return;
    }

    while(curr->next != NULL){
        printf("%s -> ", curr->val); 
        curr = curr->next;
    }
    printf("%s\n", curr->val);
}

Parameters decodeParameters(int argc, char* argv[]){
    if(argc != 3)
        error("Invalid number of parameters...\n");

    Parameters p;
    p.in_filename = argv[1];
    p.out_filename = argv[2];

    if(strcmp(p.in_filename + strlen(p.in_filename) - 5, ".text"))
        error("Input filename must be of type .text...\n");

    if(strcmp(p.out_filename + strlen(p.out_filename) - 7, ".output"))
        error("Output filename must be of type .output...\n");

    return p;
}

List* readFile(FILE* f){
    List* X = (List*) malloc(sizeof(List));
    X->head = NULL;
    X->length = 0;       //creazione lista

    char line[BUFFSIZE];
    while(fgets(line, BUFFSIZE, f) != NULL){
        line[strlen(line)-1] = '\0';    //togliamo il carattere di ritorno a capo
        insertNode(X, line);
    }
    return X;
}

short* getArray(List* X){
    short* Y = (short*) malloc(sizeof(short)*X->length);
    
    Node* curr = X->head;
    for(int i = 0; i < X->length; i++){
        Y[i] = vowelCounter(curr->val);
        curr = curr->next;
    }

    return Y;
}

double filterList(List* X, short* Y){
    double mean = 0;
    for(int i = 0; i < X->length; i++){
        mean += Y[i];
    }
    mean /= X->length;    //media degli elementi di Y

    Node* curr = X->head;
    Node* next = curr->next;
    while(curr != NULL){
        if(vowelCounter(curr->val) > mean){     //elimino il nodo se ha un numero di vocali superiori alla media
            deleteNode(X, curr->val);
        }
        curr = next;
        if(curr) next = curr->next;   //non aggiorno next se curr è già arrivato a NULL
    }

    return mean;
}

void writeToFile(FILE* f, List* X){
    Node* curr = X->head;
    while(curr != NULL){
        fputs(curr->val, f);
        fputc('\n', f);
        curr = curr->next;
    }
}

int main(int argc, char* argv[]){
    printf("============PUNTO A============\n");
    Parameters p = decodeParameters(argc,argv);
    printf("Input filename = %s, output filename = %s\n", p.in_filename, p.out_filename);

    printf("============PUNTO B============\n");
    FILE* f = fopen(p.in_filename,"r");
    List* X = readFile(f);
    printList(X);
    fclose(f);

    printf("============PUNTO C============\n");
    short* Y = getArray(X);
    for(int i = 0; i < X->length; i++){
        printf("%d ", Y[i]);
    }
    printf("\n");

    printf("============PUNTO D============\n");
    double mean = filterList(X, Y);
    printf("mean = %f\n", mean);
    printf("List contents after filtering:\n");
    printList(X);

    //=============PUNTO E==============
    f = fopen(p.out_filename, "w");
    writeToFile(f, X);
}
