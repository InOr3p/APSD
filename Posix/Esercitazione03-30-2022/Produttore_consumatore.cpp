#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

const int n = 10;
int c = 0;
int buffer[n];

pthread_mutex_t mutex;
pthread_cond_t condFull;
pthread_cond_t condEmpty;


void *produce(void* arg){
    while(true){
        sleep(0.7);
        pthread_mutex_lock(&mutex);
        while(c >= n){
            pthread_cond_wait(&condFull, &mutex);
        }

        buffer[c] = rand() % 10;
        printf("Produco. Buffer[%d] = %d\n", c, buffer[c]);
        c++;
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&condEmpty);
    }
}

void *consume(void* arg){
    while(true){
        sleep(1);
        pthread_mutex_lock(&mutex);
        while(c == 0){
            pthread_cond_wait(&condEmpty, &mutex);
        }
        
        c--;
        printf("Consumo. Buffer[%d] =  %d\n", c, buffer[c]);
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&condFull);
    }
}


int main(){
    pthread_t consumers[4];
    pthread_t producers[4];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condFull, NULL);
    pthread_cond_init(&condEmpty, NULL);

    for(int i = 0; i < 4; ++i){
        pthread_create(&consumers[i], NULL, &consume, NULL);
        pthread_create(&producers[i], NULL, &produce, NULL);
    }


    for(int i = 0; i < 4; ++i){
        pthread_join(consumers[i], NULL);
        pthread_join(producers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condFull);
    pthread_cond_destroy(&condEmpty);
}

