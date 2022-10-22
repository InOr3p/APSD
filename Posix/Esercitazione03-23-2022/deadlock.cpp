#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int num0 = 0, num1 = 0, n = 100000;
pthread_mutex_t mutex0, mutex1;

void* run0(void * arg){
	for(int i = 0; i < n; i++){
        pthread_mutex_lock(&mutex0);  //lock
        pthread_mutex_lock(&mutex1);
        num0++;  //sezione critica (eseguita in modo atomico)
        num1--;
        pthread_mutex_unlock(&mutex1);  //unlock
        pthread_mutex_unlock(&mutex0);
    }
}

void* run1(void * arg){
	for(int i = 0; i < n; i++){
        pthread_mutex_lock(&mutex1);  //deadlock!
        pthread_mutex_lock(&mutex0);  //per evitarlo: basta lockare sempre nello stesso ordine in entrambi i run(run0,run1)
        num0--;
        num1++;  //sezione critica (eseguita in modo atomico)
        pthread_mutex_unlock(&mutex0);  //unlock
        pthread_mutex_unlock(&mutex1);
    }
}

int main(int arg, char* argv[])
{
	pthread_mutex_init(&mutex0, NULL);
    pthread_mutex_init(&mutex1, NULL);
	pthread_t thid0, thid1;
	

	pthread_create(&thid0, NULL, &run0, (&num0, &num1)); //(puntatore(ID), attributi, puntatore(run), parametri(run))
    pthread_create(&thid1, NULL, &run1, (&num0, &num1));
    
    
	pthread_join(thid0, NULL); //(id, **res)
	pthread_join(thid1, NULL);

    printf("%d\n", num0);
    printf("%d\n", num1);

	pthread_mutex_destroy(&mutex0);
    pthread_mutex_destroy(&mutex1);

    return 0;
	
}