#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define ncols 20
#define nrows 20
int *readM; //usiamo un array al posto della matrice
int *writeM;
int nsteps = 50;
MPI_Datatype newt;


#define v(r,c) ((r) * (ncols / 2 + 2) + (c)) //per trasformare gli indici di matrice in indici di array

void init(int myrank){
    for(int r = 0; r < nrows; ++r)
        for(int c = 0; c < (ncols / 2) + 2; ++c)
            readM[v(r,c)] = 0;

    //inserisco un glider 
    if(myrank == 0) {
        int i = nrows / 2;
        int j = 3;

        readM[v(i-1,j)] = 1;
        readM[v(i+1,j+1)] = 1;
        readM[v(i,j+1)] = 1;
        readM[v(i+1,j)] = 1;
        readM[v(i+1,j-1)] = 1;
    }
}

void print(int step){
    printf("---%d\n", step);

    for(int r = 0; r < nrows; ++r){
        for(int c = 1; c < ncols; ++c)  // (ncols / 2) + 1
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


void transFuncInside(){
    int cont = 0; //vicini vivi
    for(int i = 0; i < nrows; ++i)
        for(int j = 1; j < (ncols / 2) + 1; ++j)
            transFunc(i,j);  

}
 

void exchBord(int myrank) {
    MPI_Request req;
    MPI_Status stat;
    int near = 0; 
    // TAG: est = 10, ovest = 11;

    if(myrank == 0) {
        near = 1;
        MPI_Isend(&readM[v(0,1)], 1, newt, near, 11, MPI_COMM_WORLD, &req);
        MPI_Isend(&readM[v(0, ncols / 2)], 1, newt, near, 10, MPI_COMM_WORLD, &req);
        MPI_Recv(&readM[v(0, 0)], 1, newt, near, 10, MPI_COMM_WORLD, &stat);
        MPI_Recv(&readM[v(0, ncols / 2 + 1)], 1, newt, near, 11, MPI_COMM_WORLD, &stat);
    }
    else{
        MPI_Isend(&readM[v(0,1)], 1, newt, near, 11, MPI_COMM_WORLD, &req);
        MPI_Isend(&readM[v(0, ncols / 2)], 1, newt, near, 10, MPI_COMM_WORLD, &req);
        MPI_Recv(&readM[v(0, 0)], 1, newt, near, 10, MPI_COMM_WORLD, &stat);
        MPI_Recv(&readM[v((nrows / 2) + 1, 0)], 1, newt, near, 11, MPI_COMM_WORLD, &stat);
    }
}


int main(int argc, char* argv[]) { 
    readM = (int*) malloc((nrows * (ncols / 2 + 2)) * sizeof(int));
    writeM = (int*) malloc((nrows * (ncols / 2 + 2)) * sizeof(int));

    MPI_Init(&argc, &argv);   
    int myrank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    init(myrank);
    double startTime = MPI_Wtime();
    MPI_Type_vector(nrows, 1, ncols / 2 + 2, MPI_INT, &newt);
    MPI_Type_commit(&newt);

    for(int s = 0; s < nsteps; s++){
        exchBord(myrank);
        print(s);
        swap();   
    }

    /*double endTime = MPI_Wtime();
    double diff = endTime - startTime;
    printf("Time: %lf", diff);
    */
    MPI_Type_free(&newt);
    MPI_Finalize();

    free(readM);
    free(writeM);
}