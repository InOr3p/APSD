#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* run(void * arg){
	sleep(1);
    int i = *(int*) arg;
    printf("%d\n", i);

    delete (int*) arg;
    return NULL;
}

int main(int arg, char* argv[])
{
    int n;
	pthread_t thid[n];
    for(int i = 0; i < n; ++i){
        int *p = new int;
        *p = i;
	    pthread_create(&thid[i], NULL, &run, p); //(puntatore(ID), attributi, puntatore(run), parametri(run))
	
    }
    
    for(int i = 0; i < n; ++i){
		pthread_join(thid[i], NULL); //(id, **res)
	}

    return 0;
	
}