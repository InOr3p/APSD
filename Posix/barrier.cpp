#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int nThread = 5;
//.... dichiarazione variabili
pthread_mutex_t mutex;
pthread_cond_t cond;
int cont = 0;

void initBarrier(){
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
}

void barrier(){
    pthread_mutex_lock(&mutex);
    cont++;

    while(cont < nThread) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_cond_broadcast(&cond);  //se cont == nThread, non entra nel while

    pthread_mutex_unlock(&mutex);
}

void destroyBarrier(){
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}


void* threadFunc(void* arg) {
    printf("inizio\n");
    barrier();
    printf("fine\n");
    return NULL;
}


int main(int argc, char* argv[]) {
    pthread_t th[nThread];
    initBarrier();

    for (int i = 0; i < nThread; i++) {
        pthread_create(&th[i], NULL, &threadFunc, NULL);
    }

    for (int i = 0; i < nThread; i++) {
        pthread_join(th[i], NULL);
    }

    destroyBarrier();
    return 0;
}