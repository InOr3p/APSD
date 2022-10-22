#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

static long num_steps = 100000000;
const double PI24 = 3.141592653589793238462643; // valore "vero" pi greco
double step;
#define num_threads 12

void main(int argc, char *argv[]){

    int i; 
    double pi, sum[num_threads], starttime, stoptime;
    step = 1.0/(double) num_steps;

    omp_set_num_threads(num_threads);

    #pragma omp parallel
    {
        int i, id, nthreads;
        double x;
        id = omp_get_thread_num();
        nthreads = omp_get_num_threads();

        if(id == 0)
            starttime = omp_get_wtime();

        for(i = id, sum[id] = 0.0; i < num_steps; i += nthreads){
            x = (i+0.5) * step;
            sum[id] += 4.0/(1.0+x*x);
        }
    }
    for(i = 0, pi = 0.0; i < num_threads; ++i){
        pi += sum[i] * step;
    }

    stoptime = omp_get_wtime();
    //printf("Pi greco: %.10f\n", pi);
    printf("With %d threads\n", num_threads);
    printf("The computed value of Pi is %2.24f\n", pi);
    printf("The \"exact\" value of Pi is %2.24f\n", PI24);
    printf("Time: %3.2f milliseconds \n", 1000*(stoptime-starttime));
}
