#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int num, n = 1000000;
pthread_mutex_t mutex;

void* run(void * arg){
	for(int i = 0; i < n; i++){
        pthread_mutex_lock(&mutex);  //lock
        num++;  //sezione critica (eseguita in modo atomico)
        pthread_mutex_unlock(&mutex);  //unlock
    }
}

int main(int arg, char* argv[])
{
	pthread_mutex_init(&mutex, NULL);
	pthread_t thid0, thid1;
	

	pthread_create(&thid0, NULL, &run, &num); //(puntatore(ID), attributi, puntatore(run), parametri(run))
    pthread_create(&thid1, NULL, &run, &num);
    
    
	pthread_join(thid0, NULL); //(id, **res)
	pthread_join(thid1, NULL);

    printf("%d\n", num);

	pthread_mutex_destroy(&mutex);

    return 0;
	
}