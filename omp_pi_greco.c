#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

static long num_steps = 100000000;
double step;
#define num_threads 4

void main(int argc, char *argv[]){

    int i; 
    double pi, sum[num_threads];
    step = 1.0/(double) num_steps;

    omp_set_num_threads(num_threads);

    #pragma omp parallel
    {
        int i, id, nthreads;
        double x;
        id = omp_get_thread_num();
        nthreads = omp_get_num_threads();

        for(i = id, sum[id] = 0.0; i < num_steps; i += nthreads){
            x = (i+0.5) * step;
            sum[id] += 4.0/(1.0+x*x);
        }
    }
    for(i = 0; i < num_threads; ++i){
        pi += sum[i] * step;
    }

    printf("Pi greco: %.10f\n", pi);
}
