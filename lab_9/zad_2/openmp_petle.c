#include<stdlib.h>
#include<stdio.h>
#include<omp.h>

int main() {

#ifdef   _OPENMP
    printf("\nKompilator rozpoznaje dyrektywy OpenMP\n");
#endif

    int liczba_watkow = 4;
    int id, i;
    omp_set_num_threads(liczba_watkow);

#pragma omp parallel for ordered schedule(static)
    for (int i = 0; i < 17; i++) {
        int id = omp_get_thread_num();
#pragma omp ordered
        printf("Iteracja %d wykonana przez watek nr. %d.\n", i, id);
    }
}