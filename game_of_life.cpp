#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define ncols 20
#define nrows 20
int *readM = new int[((nrows / 2) + 2) * ncols]; //usiamo un array al posto della matrice
int *writeM = new int[((nrows / 2) + 2) * ncols];
int nsteps = 50;

#define v(r,c) ((r) * ncols + (c)) //per trasformare gli indici di matrice in indici di array

void init(int myrank){
    for(int r = 0; r < (nrows / 2) + 2; ++r)
        for(int c = 0; c < ncols; ++c)
            readM[v(r,c)] = 0;

    //inserisco un glider 
    if(myrank == 0) {
        int i = 3;
        int j = ncols / 2;

        readM[v(i-1,j)] = 1;
        readM[v(i+1,j+1)] = 1;
        readM[v(i,j+1)] = 1;
        readM[v(i+1,j)] = 1;
        readM[v(i+1,j-1)] = 1;
    }
}

void print(int step){
    printf("---%d\n", step);

    for(int r = 1; r <(nrows / 2) + 1; ++r){
        for(int c = 0; c < ncols; ++c)
            printf("%d", readM[v(r,c)]);
        printf("\n");
    }
}

void swap(){
    int *p = readM;
    readM = writeM;
    writeM = p;
}

 void transFunc(int i, int j){
    int cont = 0; //vicini vivi
    for(int di = -1; di < 2; ++di)
        for(int dj = -1; dj < 2; ++dj)
            if(!(di == 0 && dj == 0) && readM[v((i+di + nrows) %nrows, (j+dj + ncols) % ncols)])
                cont++;
    
    if(readM[v(i, j)] == 1)
        if(cont == 2 || cont == 3)
            writeM[v(i, j)] = 1;
        else
            writeM[v(i, j)] = 0;

    else 
        if(cont == 3)
            writeM[v(i, j)] = 1;
        else
            writeM[v(i, j)] = 0;  

}
 

void exchBord(int myrank) {
    MPI_Request req;
    MPI_Status stat;
    int near = 0;

    if(myrank == 0) {
        near = 1;
        MPI_Isend(&readM[v(1,0)], ncols, MPI_INT, near, 0, MPI_COMM_WORLD, &req);
        MPI_Isend(&readM[v(nrows / 2, 0)], ncols, MPI_INT, near, 1, MPI_COMM_WORLD, &req);
        MPI_Recv(&readM[v(0, 0)], ncols, MPI_INT, near, 1, MPI_COMM_WORLD, &stat);
        MPI_Recv(&readM[v((nrows / 2) + 1, 0)], ncols, MPI_INT, near, 0, MPI_COMM_WORLD, &stat);
    }
    else{
        MPI_Isend(&readM[v(1,0)], ncols, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);
        MPI_Isend(&readM[v(nrows / 2, 0)], ncols, MPI_INT, 0, 1, MPI_COMM_WORLD, &req);
        MPI_Recv(&readM[v(0, 0)], ncols, MPI_INT, 0, 1, MPI_COMM_WORLD, &stat);
        MPI_Recv(&readM[v((nrows / 2) + 1, 0)], ncols, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
    }
}


int main(int argc, char* argv[]) { 
    MPI_Init(&argc, &argv);   
    int myrank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    init(myrank);
    for(int s = 0; s < nsteps; s++){
        print(s);
        exchBord(myrank);
        for(int i = 1; i < (nrows / 2) + 1; i++)
            for(int j = 0; j < ncols; j++)
                transFunc(i,j);
        swap();
    }

    delete [] readM;
    delete [] writeM;
    MPI_Finalize();
    
}