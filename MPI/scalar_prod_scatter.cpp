#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "mpi.h"
using namespace std;

#define SIZE 10

void printArray(int* arr, int s, int myrank, int numArray) { // stampa di array arr
    printf("RANK: %d - ARRAY %d ---> |", myrank , numArray);
    for(int i = 0; i < s; i++) {
        printf("%d|", arr[i]);
    }
    printf("\n");
}

void init(int* arr, int numArray) {  // inizializzazione
    printf("Inserire array %d\n", numArray);
    for(int i = 0; i < SIZE; ++i)
        scanf("%d", &arr[i]);
}

int scalar_product(int local1[], int local2[], int size) {
    int sum = 0;
    for(int i = 0; i < size; i++) {
        sum += (local1[i] * local2[i]); 
    }
    printf("Risultato parziale: %d\n", sum);
    return sum;
}


int main(int argc, char* argv[]) {
    int res = 0, numThreads, myrank, numsPerThreads, sum = 0;
    int array1[SIZE], array2[SIZE];
    int* local_array1;
    int* local_array2;
    

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numThreads);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if(SIZE % numThreads == 0) {
        if(myrank == 0) {
            printf("Inizializzazione...\n");
            init(array1, 1);
            init(array2, 2);
            printArray(array1, SIZE, 0, 0);
            printArray(array2, SIZE, 0, 1);
        }

        numsPerThreads = SIZE / numThreads;
        local_array1 = new int[numsPerThreads];
        local_array2 = new int[numsPerThreads];

        printf("Scatter1...\n");
        MPI_Scatter(array1, numsPerThreads, MPI_INT, local_array1, numsPerThreads, MPI_INT, 0, MPI_COMM_WORLD);
        printf("Scatter2...\n");
        MPI_Scatter(array2, numsPerThreads, MPI_INT, local_array2, numsPerThreads, MPI_INT, 0, MPI_COMM_WORLD);
        printArray(local_array1, numsPerThreads, myrank, 1);
        printArray(local_array2, numsPerThreads, myrank, 2);

        sum = scalar_product(local_array1, local_array2, numsPerThreads);

        MPI_Reduce(&sum, &res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        if(myrank == 0)
            printf("Il prodotto scalare è: %d\n", res);
            
        delete [] local_array1;
        delete [] local_array2;
    }
    
    else if(myrank == 0)
        printf("Il numero di processi usati deve essere divisibile per la dimensione degli array %d\n", SIZE);

    MPI_Finalize();
}