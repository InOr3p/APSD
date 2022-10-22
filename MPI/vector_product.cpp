#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define N 5
#define M 4

void init(int matrix[][N], int* x) {
    for(int i = 0; i < M; ++i) 
        for(int j = 0; j < N; ++j) {
            
            if(i == 0)
                x[j] = j + 1;

            matrix[i][j] = j + 1;
        }
}


int scalar_product(int local1[], int local2[]) {
    int sum = 0;
    for(int i = 0; i < N; i++) {
        sum += (local1[i] * local2[i]); 
    }
    printf("Risultato parziale: %d\n", sum);
    return sum;
}


int main(int argc, char* argv[]) {
    int A[M][N];
    int x[N], y[M];
    int myrank, numThreads, elementsPerThread, start, end, sum = 0, res = 0;
    MPI_Status status;
    //MPI_Datatype datatype;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &numThreads);

    elementsPerThread = M / numThreads;
    start = elementsPerThread * myrank;
    end = start + elementsPerThread;
        
    if(M % numThreads == 0) {
        if(myrank == 0)
            init(A, x);

        MPI_Bcast(x, N, MPI_INT, 0, MPI_COMM_WORLD); //tutti i processi devono visualizzare l'array x
        if(myrank == 0) {
            int temp_rank = 0;
            for(int i = end; i < M; ++i) {
                if(i % elementsPerThread == 0)
                    temp_rank++;
                MPI_Send(A[i], N, MPI_INT, temp_rank, 0, MPI_COMM_WORLD);
            }
        }
        

        for(int i = start; i < end; ++i) {
            if(myrank != 0)
                MPI_Recv(A[i], N, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            y[i] = scalar_product(A[i], x);
        }

        MPI_Gather(&y[start], elementsPerThread, MPI_INT, &y[start], elementsPerThread, MPI_INT, 0, MPI_COMM_WORLD);

        if(myrank == 0) {
            printf("Il prodotto scalare è: ");
            //stampa array y
            printf("|");
            for(int i = 0; i < M; ++i)
                printf("%d|", y[i]);
            printf("\n");
        }

        
    }

    else if(myrank == 0) {
        printf("Inserire un numero di processi divisibile per la dimensione dell'array y %d\n", M);
    }

    MPI_Finalize();
}