#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define ncols 20
#define nrows 20
int *readM; //usiamo un array al posto della matrice
int *writeM;
int nsteps = 50;

#define v(r,c) ((r) * ncols + (c)) //per trasformare gli indici di matrice in indici di array

void init(){
    readM = malloc(nrows * ncols * sizeof(int));
    writeM = malloc(nrows * ncols * sizeof(int));

    for(int r = 0; r < nrows; ++r)
        for(int c = 0; c < ncols; ++c)
            readM[v(r,c)] = 0;

    //inserisco un glider  
    int i = nrows / 2;
    int j = ncols / 2;

    readM[v(i-1,j)] = 1;
    readM[v(i+1,j+1)] = 1;
    readM[v(i,j+1)] = 1;
    readM[v(i+1,j)] = 1;
    readM[v(i+1,j-1)] = 1;
}

void print(int step){
    printf("---%d\n", step);

    for(int r = 0; r < nrows; ++r){
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
            if(!(di == 0 && dj == 0) && readM[v((i+di + nrows) % nrows, (j + dj + ncols) % ncols)])
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
 

int main(int argc, char* argv[]) { 
    MPI_Init(&argc, &argv);   
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    init();
    for(int s = 0; s < nsteps; s++){
        print(s);
        for(int i = 0; i < nrows; i++)
            for(int j = 0; j < ncols; j++)
                transFunc(i,j);
        swap();
    }

    free(readM);
    free(writeM);
    MPI_Finalize();
}