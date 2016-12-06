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
    printf("STATIC\n");
#pragma omp parallel for ordered schedule(static)
    for (int i = 0; i < 17; i++) {
        int id = omp_get_thread_num();
#pragma omp ordered
        printf("Iteracja %d wykonana przez watek nr. %d.\n", i, id);
    }
printf("\n");
    printf("STATIC 3\n");
#pragma omp parallel for ordered schedule(static, 3)
    for (int i = 0; i < 17; i++) {
        int id = omp_get_thread_num();
#pragma omp ordered
        printf("Iteracja %d wykonana przez watek nr. %d.\n", i, id);
    }
    printf("\n");
    printf("DYNAMIC\n");
#pragma omp parallel for ordered schedule(dynamic)
    for (int i = 0; i < 17; i++) {
        int id = omp_get_thread_num();
#pragma omp ordered
        printf("Iteracja %d wykonana przez watek nr. %d.\n", i, id);
    }
    printf("\n");
    printf("DYNAMIC 3\n");
#pragma omp parallel for ordered schedule(dynamic, 3)
    for (int i = 0; i < 17; i++) {
        int id = omp_get_thread_num();
#pragma omp ordered
        printf("Iteracja %d wykonana przez watek nr. %d.\n", i, id);
    }
    printf("\n");
    printf("GUIDED\n");
#pragma omp parallel for ordered schedule(guided)
    for (int i = 0; i < 17; i++) {
        int id = omp_get_thread_num();
#pragma omp ordered
        printf("Iteracja %d wykonana przez watek nr. %d.\n", i, id);
    }
    printf("\n");
    printf("GUIDED 4\n");
#pragma omp parallel for ordered schedule(guided, 4)
    for (int i = 0; i < 17; i++) {
        int id = omp_get_thread_num();
#pragma omp ordered
        printf("Iteracja %d wykonana przez watek nr. %d.\n", i, id);
    }
}