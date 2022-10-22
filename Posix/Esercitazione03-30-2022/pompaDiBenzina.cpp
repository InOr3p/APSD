#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


int fuel = 0, numT = 6;
pthread_mutex_t mutex;
pthread_cond_t cond;


void *filler(void* arg){
    while(true){
        sleep(1);
        pthread_mutex_lock(&mutex);
        fuel += 30;
        printf("Fuel: %d\n", fuel);
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&cond);
    }
}


void *car(void* arg){
    while(true){
        sleep(1);
        pthread_mutex_lock(&mutex);
        while(fuel < 50){
            pthread_cond_wait(&cond, &mutex); //un thread rilascia il mutex e si mette a dormire
        }                                     //quando si risveglia (con call di broadcast), riprende automaticamente il mutex

        fuel -= 50;
        printf("Fuel: %d\n", fuel);
        pthread_mutex_unlock(&mutex);
    }
}


int main(){
    pthread_t th[numT];
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    for(int i = 0; i < numT; ++i)
        if(i < numT / 2)
            pthread_create(&th[i], NULL, &filler, NULL);
        
        else pthread_create(&th[i], NULL, &car, NULL);

    
    for(int i = 0; i < numT; i++)
        pthread_join(th[i], NULL);
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}