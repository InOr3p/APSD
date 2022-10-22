#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <unistd.h>



//.... dichiarazione variabili

int cont = 0; //viene scambiato tra i processi
MPI_Status status;
MPI_Request req;

void barrier(int rank, int size){

    if(rank == 0) {
        for(int i = 1; i < size; ++i) {
            MPI_Recv(&cont, 1, MPI_INT, i, 5, MPI_COMM_WORLD, &status);
        }
        for(int i = 1; i < size; ++i) {
            MPI_Isend(&cont, 1, MPI_INT, i, 6, MPI_COMM_WORLD, &req);
        }
    }
    else {
        MPI_Isend(&cont, 1, MPI_INT, 0, 5, MPI_COMM_WORLD, &req);
        MPI_Recv(&cont, 1, MPI_INT, 0, 6, MPI_COMM_WORLD, &status);
    }

    /*for(int i = 0; i < size; ++i) {
        if(rank != i) {
            MPI_Isend(&cont, 1, MPI_INT, i, 5, MPI_COMM_WORLD, &req);
        }
    }

    for(int i = 0; i < size; ++i) {
        if(rank != i) {
            //printf("sto per ricevere i mex... rank: %d\n", rank);
            MPI_Irecv(&cont, 1, MPI_INT, i, 5, MPI_COMM_WORLD, &req);
            MPI_Wait(&req, &status);
            //sleep(0.5);
        }
    }*/

}

void threadFunc(int rank, int size) {
    printf("rank: %d -- inizio\n", rank);
    barrier(rank, size);
    printf("rank: %d -- fine\n", rank);
}


int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    threadFunc(rank, size);
    MPI_Finalize();
    return 0;
}