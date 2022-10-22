#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUMTHREADS 4
#define N 10
int arr[N], sums[NUMTHREADS] = {0};
int res = 0, id = 0;
pthread_mutex_t mutex;

void* run(void* arg) {
    pthread_mutex_lock(&mutex);
    int thid = id++;
    pthread_mutex_unlock(&mutex);

    int start = (N / NUMTHREADS) * thid;
    int end = start + (N / NUMTHREADS);
    int j;

    for(j = start; j < end; ++j) {
        printf("sums[%d] += %d\n", thid, arr[j]);
        sums[thid] += arr[j];
    }
    
    if(thid == (NUMTHREADS - 1) && j < N) {
        for(int i = j; i < N; ++i)
            sums[thid] += arr[i];
    }
    printf("thid: %d ---> %d\n", thid, sums[thid]);
    return NULL;
}


void init() {
    printf("Inserire elementi in array\n");
    for(int i = 0; i < N; ++i)
        scanf("%d", &arr[i]);
}

int main(int argc, char* argv[]) {
    pthread_mutex_init(&mutex, NULL);
    init();
    pthread_t threads[NUMTHREADS];
    for(int i = 0; i < NUMTHREADS; ++i)
        pthread_create(&threads[i], NULL, &run, NULL);


    for(int i = 0; i < NUMTHREADS; ++i) 
        pthread_join(threads[i], NULL);
    
    for(int i = 0; i < NUMTHREADS; ++i)
        res += sums[i];

    printf("Result: %d\n", res);
    pthread_mutex_destroy(&mutex);
}
