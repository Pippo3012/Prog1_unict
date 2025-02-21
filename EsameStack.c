#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct Parameters{
    char file[256];
    int k;
} Parameters;

typedef struct Node{
    char *data;
    struct Node *next;
} Node;

Parameters readInput(int argc, char *argv[]);
void pushWord(Node **stack, const char *word);
char *popWord(Node **stack);
Node *buildStack(const char *filename);
char *sortWord(const char *word);
void printStack(Node **stack, int k);
void freeStack(Node **stack);

int main(int argc, char *argv[]){

    Parameters params = readInput(argc, argv);

    Node *stack = buildStack(params.file);

    printStack(&stack, params.k);

    freeStack(&stack);

    return 0;

  }

Parameters readInput(int argc, char *argv[]){
    if(argc != 3){
        fprintf(stderr, "Errore: parametri insufficienti\n");
        exit(1);
    }
    Parameters params;
    strncpy(params.file, argv[1], (sizeof(params.file)-1));
    params.file[256 - 1] = '\0';
    params.k = atoi(argv[2]);
    if(params.k < 5 || params.k > 15){
        fprintf(stderr, "Errore: parametri incorretti (k)\n");
        exit(1);
    }
    return params;
}

void pushWord(Node **stack, const char *word){
    Node *newNode = (Node*)malloc(sizeof(Node));
    if(!newNode){
        fprintf(stderr, "Errore: errore durante l'allocazione della memoria\n");
        exit(1);
    }
    newNode->data = strdup(word);
    if(!newNode->data){
        fprintf(stderr, "Errore: errore durante l'allocazione della memoria\n");
        exit(1);
    }
    newNode->next = *stack;
    *stack = newNode;
}

char *popWord(Node **stack){ 
    if(*stack == NULL){
        return NULL;
    }
    Node *temp = *stack;
    char *word = temp->data;
   *stack = temp->next;
    free(temp);
    return word;
}

Node *buildStack(const char *filename){
    FILE *fp = fopen(filename, "r");
    if(!fp){
        fprintf(stderr, "Errore: errore durante l'apertura del file\n");
        exit(1);
    }
    Node *stack = NULL;
    char buffer[31];
    while(fgets(buffer, sizeof(buffer), fp)){
        buffer[strcspn(buffer, "\n")] = '\0'; // rimuovo il carattere newline
        pushWord(&stack, buffer);
    }
    fclose(fp);
    return stack;
}

char *sortWord(const char *word){
    char *sorted = strdup(word);
    if(!sorted){
        fprintf(stderr, "Errore: memoria insufficiente\n");
        exit(1);
    }
    size_t len = strlen(sorted);
    for(size_t i=0; i<len; i++){
        for(size_t j=i+1; j<len; j++){
            if(sorted[i]>sorted[j]){
                char temp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = temp;
            }
        }
    }
    return sorted;
}

void printStack(Node **stack, int k){
    while(*stack){
        char *word = popWord(stack);
        if(strlen(word)>=(size_t)k){
            char *sorted = sortWord(word);
            printf("%s\n", sorted);
            free(sorted);
        } else{
            printf("%s\n", word);
        }
        free(word);
    }
}

void freeStack(Node **stack){
    while(*stack){
        Node *temp = *stack;
        free(temp->data);
        *stack = temp->next;
        free(temp);
    }
}
