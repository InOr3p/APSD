#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define size 1000000

int main(int argc, char* argv[]) { 
    MPI_Init(&argc, &argv);   
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int *v = new int[size];

    if(rank == 0){
        for(int i = 0; i < size; i++)
            v[i] = 222;
        MPI_Request req;
        MPI_Isend(&v[0], size, MPI_INT, 1, 3, MPI_COMM_WORLD, &req);
        //v[0] = 666;
        MPI_Status stat;
        MPI_Wait(&req, &stat);
    }

    else{
        MPI_Status stat;
        MPI_Recv(v, size, MPI_INT, 0, 3, MPI_COMM_WORLD, &stat);
        printf("%d", v[0]);
    }

    delete [] v;
    MPI_Finalize();
    
}