#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 10
#define steps 5
#define numFormiche 2

int celle[SIZE];


class Formica {
public:
    int pos;
    int dir;
    int idForm;

    Formica(int i) {
        idForm = i;
        srand(time(0));
        pos = rand() % SIZE;
        dir = rand() % 2;
        celle[pos] = idForm;        
    }
};

int idThread = 0;
pthread_t threads[numFormiche];
Formica formiche[numFormiche];
pthread_mutex_t mutex;

void init() {
    for(int i = 0; i < SIZE; ++i)
        celle[i] = -1;
    
    for(int i = 0; i < numFormiche; ++i)
        formiche[i] = Formica(i);    
}


void printArray() {
    printf("----- Stampa array -----\n|");
    for(int i = 0; i < SIZE; ++i) {
        if(celle[i] != -1)
            printf("%d|", 1);
        else printf("%d|", 0);
    }
    printf("\n");
}

void* move(void* arg) {
    Formica f =  *(Formica*) arg;

    pthread_mutex_lock(&mutex);

    if(f.dir == 0) {
        if(f.pos == 0 || celle[f.pos - 1] != -1)
            f.dir = 1;
        else {
            celle[f.pos - 1] = f.idForm;
            celle[f.pos] = -1;
            f.pos--;
        }
    }
    else {
        if(f.pos == (SIZE - 1) || celle[f.pos + 1] != -1)
            f.dir = 0;
        else {
            celle[f.pos + 1] = f.idForm;
            celle[f.pos] = -1;
            f.pos++;
        }
    }

    printArray();
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int trovaFormica(int pos) {
    for(int i = 0; i < numFormiche; ++i)
        if(formiche[i].pos == pos)
            return i;
    return -1;
}

int main(int argc, char* argv[]) {
    init();
    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < steps; ++i)
        for(int j = 0; j < SIZE; j++)
            if(celle[j] != -1) {
                int index = trovaFormica(j);
                pthread_create(&threads[idThread++], NULL, &move, &formiche[index]);
            }

    for(int i = 0; i < numFormiche; ++i) 
        pthread_join(threads[i], NULL);
    
    pthread_mutex_destroy(&mutex);
}