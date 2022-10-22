#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

#define N 10
int *arr;

void printArray() {
    printf("-----Stampa del vettore-----\n");
    printf("|");
    for(int i = 0; i < N; i++) {
        printf("%d|", arr[i]);
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    arr = malloc(N * sizeof(int));
    int numThreads, myrank, numsPerThread, sum = 0, result = 0;
    for(int i = 0; i < N; i++) {
        arr[i] = i;
    }
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &numThreads);
    numsPerThread = N / numThreads;
   
    if(myrank == 0) {
        printArray();
        printf("Numero di threads utilizzati: %d\n", numThreads);
        printf("Numero di elementi da processare per thread: %d\n", numsPerThread);
    } 

    int start = myrank * numsPerThread;
    int end = start + numsPerThread;
    int i;
    for(i = start; i < end; ++i) {
        sum += arr[i];
    }
    
    if(myrank == (numThreads - 1) && i < N) {
        for(int j = i; j < N; ++j)
            sum += arr[j];
    }
    
    MPI_Reduce(&sum, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    /*if(myrank == 0) {
        result += sum;
        for(int i = 1; i < numThreads; i++) {
            MPI_Recv(&sum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            result += sum;
        }
    }
    else{
        MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }*/

    if(myrank == 0)
        printf("Somma totale: %d\n", result);

    MPI_Finalize();
    free(arr);
}