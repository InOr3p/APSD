#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10
#define numThreads 4


void init(int a[]) {
    printf("Inserire array:\n");
    for(int i = 0; i < N; ++i) {
        scanf("%d", &a[i]);
    }
}


int main(int argc, char* argv[]) {

    int arr1[N], arr2[N];
    int res = 0;

    init(arr1);
    init(arr2);

    omp_set_num_threads(numThreads);

    #pragma omp parallel 
    {
        int sum = 0;

        #pragma omp for 
            for(int i = 0; i < N; ++i)
                sum += arr1[i] * arr2[i];

        printf("Somma parziale di thread %d: %d\n", omp_get_thread_num(), sum);
        #pragma omp critical 
        {
            res += sum;
        }
    }

    printf("Il prodotto scalare è: %d\n", res);
}