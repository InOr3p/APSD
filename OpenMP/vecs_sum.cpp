// vec sum
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10000

int main (int argc, char *argv[]) {
    double a[N];
    double b[N];
    double c[N];
    double starttime, stoptime;
    int i, tid, threads;
    bool first_thread = false;

    omp_set_num_threads(4);
    /* Initialization */
    #pragma omp parallel shared(a, b, c, first_thread) private(tid, i) //num_threads(4)
    {
        tid = omp_get_thread_num();
        threads = omp_get_num_threads();
        #pragma omp for
        for (i=0; i < N; i++)
            a[i] = b[i] = (double)i;
        // barrier implicita!
        //#pragma omp barrier
        
        #pragma omp critical
        {
            if (!first_thread){
                starttime = omp_get_wtime();
                first_thread = true;
            }
        }

        #pragma omp for // schedule(dynamic) // static?
        for (i=0; i < N; i++) {
            c[i] = a[i] + b[i];
        }
    } // parallel section

    stoptime = omp_get_wtime();

    printf("starttime: %.5f \nstoptime: %.5f", starttime, stoptime);
    // verifica seq
    // int error = 0;
    // for (i=0; i < N; i++) {
    // if (c[i] != 2 * i)
    // error = 1;
    // }
    // if (error == 1)
    // printf("ERRORE di inconsistenza se/par!!!\n");
    printf("With %d threads\n", threads);
    printf("We have Time: %3.5f milliseconds \n", (stoptime-starttime)*1000);
    for(int i: c)
        printf("%d ", i);
    return 0;
}