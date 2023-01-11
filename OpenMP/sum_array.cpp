#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10
#define NTHREADS 4

int main(int argc, char* argv[]) {


    // #pragma omp parallel shared(a) {
    //     #pragma omp for
    //         for(int i = 0; i < N; i++)
    //             #pragma omp critical
    //                 sum = sum + a[i];
    //}

    int a[N] = {1,2,3,4,5,6,7,8,9,10};
    int localsum[NTHREADS] = {0};
    int totalsum = 0, id;

    for(int i = 0; i < NTHREADS; i++)
        printf("Localsum[%d]: %d\n", i, localsum[i]);

    omp_set_num_threads(NTHREADS);

    #pragma omp parallel private(id)
    {
        id = omp_get_thread_num();

        #pragma omp for
        for(int i = 0; i < N; i++)
            localsum[id] += a[i];

        printf("Thread %d --- Somma locale: %d\n", id, localsum[id]);

        #pragma omp for
        for(int i = 0; i < NTHREADS; i++)
            totalsum += localsum[i];
    }

    printf("Somma totale: %d\n", totalsum);

}