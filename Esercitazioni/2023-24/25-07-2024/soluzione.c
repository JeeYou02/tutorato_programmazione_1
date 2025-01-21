#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFSIZE 256

typedef struct{
    char* filename;
}Parameters;

typedef struct{
    int n,m;
}Sizes;

void error(char* err){
    fprintf(stderr, "%s", err);
    exit(1);
}

Parameters decodeParameters(int argc, char* argv[]){
    if(argc != 2)
        error("Invalid number of parameters...\n");

    char* suffix = ".csv";
    
    Parameters p;
    p.filename = argv[1];

    if(strcmp(p.filename + strlen(p.filename) - 4, suffix))
        error("File type must be .csv");

    return p;
}

Sizes readHeader(FILE* f){
    char buff[BUFFSIZE];
    fgets(buff, BUFFSIZE, f);

    Sizes s;
    s.n = atoi(strtok(buff, ","));
    s.m = atoi(strtok(NULL, ","));

    return s;
}

short*** readFile(FILE* f, int n, int m){
    short*** X = (short***) malloc(sizeof(short**) * n);
    for(int i = 0; i < n; i++) X[i] = (short**) malloc(sizeof(short*) * m);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            X[i][j] = (short*) malloc(sizeof(short));
        }
    }

    for(int i = 0; i < n; i++){
        char line[BUFFSIZE];
        fgets(line, BUFFSIZE, f);
        *X[i][0] = atoi(strtok(line,","));
        for(int j = 1; j < m; j++){
            *X[i][j] = atoi(strtok(NULL,","));
        }
    }

    return X;
}

short* getArray(short*** X, int n, int m){
    short* Y = (short*) malloc(sizeof(short) * n);

    for(int i = 0; i < n; i++){
        short max = *X[i][0];
        for(int j = 1; j < m; j++){
            if(*X[i][j] > max)
                max = *X[i][j];
        }
        Y[i] = max;
    }

    return Y;
}

void bubbleSort(short* Y, int n){
    for(int i = 0; i < n-1; i++){
        for(int j = 0; j < n-i-1; j++){
            if(Y[j] > Y[j+1]){
                short aux = Y[j];
                Y[j] = Y[j+1];
                Y[j+1] = aux;
            }
        }
    }
}

void processX(short*** X, short* Y, int n, int m){
    for(int i = 0; i < n; i++){
        double soglia = Y[i]/2 + 1;
        for(int j = 0; j < m; j++){
            if(*X[i][j] > soglia){
                free(X[i][j]);
                X[i][j] = NULL;
            }
        }
    }
}

int main(int argc, char* argv[]){
    Parameters p = decodeParameters(argc, argv);
    printf("=============PUNTO A=============\n");
    printf("Filename = %s\n", p.filename);


    FILE* f = fopen(p.filename, "r");
    Sizes s = readHeader(f);
    printf("=============PUNTO B=============\n");
    printf("n = %d, m = %d\n", s.n, s.m);

    short*** X = readFile(f, s.n, s.m);
    printf("Matrix X:\n");
    for(int i = 0; i < s.n; i++){
        for(int j = 0; j < s.m; j++){
            printf("%d ", *X[i][j]);
        }
        printf("\n");
    }

    printf("\n");

    printf("=============PUNTO C=============\n");
    short* Y = getArray(X, s.n, s.m);

    printf("Y before bubblesort:\n");
    for(int i = 0; i < s.n; i++)
        printf("%d ", Y[i]);

    printf("\n");

    bubbleSort(Y, s.n);

    printf("Y after bubblesort:\n");
    for(int i = 0; i < s.n; i++)
        printf("%d ", Y[i]);

    printf("\n");

    printf("=============PUNTO D=============\n");
    processX(X, Y, s.n, s.m);
    for(int i = 0; i < s.n; i++){
        for(int j = 0; j < s.m; j++){
            if(X[i][j] == NULL){
                printf("* ");
            } else {
                printf("%d ", *X[i][j]);
            }
        }
        printf("\n");
    }

    printf("\n");

    printf("=============PUNTO E=============\n");
    for(int i = 0; i < s.n; i++){
        for(int j = 0; j < s.m; j++){
            if(X[i][j] == NULL){
                fprintf(stderr, "- ");
            } else {
                fprintf(stderr, "%d ", *X[i][j]);
            }
        }
        fprintf(stderr, "\n");
    }

    free(X);
    fclose(f);
}
