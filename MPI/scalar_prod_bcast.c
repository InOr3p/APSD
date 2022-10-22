#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

#define SIZE 10

int array1[SIZE], array2[SIZE];
int res = 0, sum = 0, numThreads, myrank, numsPerThreads, start, end;

void printArray(int* arr, int s) { // stampa di array arr
    printf("myrank: %d\n", myrank);
    printf("|");
    for(int i = 0; i < s; i++)
        printf("%d|", arr[i]);
    printf("\n");
}

void init(int* arr, int numArray) {  // inizializzazione
    if(myrank == 0) {
        printf("Inserire array %d\n", numArray);
        for(int i = 0; i < SIZE; ++i)
            scanf("%d", &arr[i]);
    
        printArray(arr, SIZE);
    }

    MPI_Bcast(arr, SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    //printArray(arr, SIZE);
}

int scalar_product() {
    numsPerThreads = SIZE / numThreads;
    start = numsPerThreads * myrank;
    end = numsPerThreads + start;

    if(myrank == (numThreads - 1) && end < SIZE) {
        end = SIZE;
    }

    for(int i = start; i < end; ++i) {
        sum += array1[i] * array2[i];
    }

    printf("Somma parziale di thread %d: %d\n", myrank, sum);
    return sum;

}


int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numThreads);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    
    init(array1, 1);
    init(array2, 2);

    scalar_product();

    MPI_Reduce(&sum, &res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(myrank == 0)
        printf("Il prodotto scalare è: %d\n", res);    
    
    MPI_Finalize();
}