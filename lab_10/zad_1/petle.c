#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define WYMIAR 10

int main () {

    double a[WYMIAR][WYMIAR];
    int n, i, j;

    for (i = 0; i < WYMIAR; i++) for (j = 0; j < WYMIAR; j++) a[i][j] = 1.02 * i + 1.01 * j;

    n = WYMIAR;

    double suma = 0.0;
    for (i = 0; i < WYMIAR; i++) {
        for (j = 0; j < WYMIAR; j++) {
            suma += a[i][j];
        }
    }

    printf("Suma wyrazow tablicy: %lf\n", suma);

    omp_set_nested(1);
    int num_threads = 5;
    omp_set_num_threads(num_threads);

    //WIERSZOWO
    printf("\nWIERSZOWO\n");

    double suma_parallel_wierszowo = 0.0;
    double suma_loc = 0.0;

#pragma omp parallel for ordered default(none) shared(suma_parallel_wierszowo, a) private(i, j, suma_loc)
        for (i = 0; i < WYMIAR; i++) {
            suma_loc = 0.0;
            for (j = 0; j < WYMIAR; j++) {
                suma_loc += a[i][j];
#pragma omp ordered
                printf("kolumna %d, wiersz %d: watek %d\n", j, i, omp_get_thread_num());
            }
#pragma omp atomic
            suma_parallel_wierszowo+=suma_loc;
        }

    printf("Suma wyrazow tablicy rownolegle wierszowo: %lf\n", suma_parallel_wierszowo);

    //KOLUMNOWO REDUCTION
    printf("\nKOLUMNOWO REDUCTION\n");

    double suma_parallel_kolumnowo_reduction = 0.0;

    for (i = 0; i < WYMIAR; i++) {
#pragma omp parallel for ordered reduction(+:suma_parallel_kolumnowo_reduction) private(j) shared(a) firstprivate(i)
        for (j = 0; j < WYMIAR; j++) {
            suma_parallel_kolumnowo_reduction += a[i][j];
#pragma omp ordered
            printf("kolumna %d, wiersz %d: watek %d\n", j, i, omp_get_thread_num());
        }
    }

    printf("Suma wyrazow tablicy rownolegle kolumnowo reduction: %lf\n", suma_parallel_kolumnowo_reduction);

    //KOLUMNOWO PRIVATE
    printf("\nKOLUMNOWO PRIVATE\n");

    double suma_parallel_kolumnowo_private = 0.0;
    double sum = 0.0;

#pragma omp parallel default(none) shared(suma_parallel_kolumnowo_private, a) private(i, j, sum)
    {
        sum = 0.0;
#pragma omp for ordered
        for (i = 0; i < WYMIAR; i++) {
            for (j = 0; j < WYMIAR; j++) {
                sum += a[j][i];
#pragma omp ordered
                printf("kolumna %d, wiersz %d: watek %d\n", i, j, omp_get_thread_num());

            }
        }
#pragma atomic
            suma_parallel_kolumnowo_private+=sum;

    }

    printf("Suma wyrazow tablicy rownolegle kolumnowo private: %lf\n", suma_parallel_kolumnowo_private);

    //BLOKOWO
    printf("\n2D\n");

    double suma_parallel_blokowo = 0.0;
    int id_zewn;

    int chunk = floor(WYMIAR/num_threads);
#pragma omp parallel default(none) shared(a, suma_parallel_blokowo, chunk) private(i, j ,id_zewn)
    {
#pragma omp for ordered schedule(static, chunk)
        for(i=0;i<WYMIAR;i++) {
            id_zewn = omp_get_thread_num();
#pragma omp ordered
#pragma omp parallel for ordered schedule(static) default(none) firstprivate(id_zewn, i) shared(a, suma_parallel_blokowo)
            for(j=0;j<WYMIAR;j++) {
#pragma omp atomic
                suma_parallel_blokowo += a[i][j];
#pragma omp ordered
                printf("kol %d, wie %d: watek: zewn %d, wew %d\n", j, i, id_zewn, omp_get_thread_num());
            }
        }
    }
    printf("Suma wyrazow tablicy rownolegle blokowo: %lf\n", suma_parallel_blokowo);


    printf("\nNA KONIEC:\n");
    printf("Suma wyrazow tablicy: %lf\n", suma);
    printf("Suma wyrazow tablicy rownolegle wierszowo: %lf\n", suma_parallel_wierszowo);
    printf("Suma wyrazow tablicy rownolegle kolumnowo reduction: %lf\n", suma_parallel_kolumnowo_reduction);
    printf("Suma wyrazow tablicy rownolegle kolumnowo private: %lf\n", suma_parallel_kolumnowo_private);
    printf("Suma wyrazow tablicy rownolegle blokowo: %lf na %d watkach\n", suma_parallel_blokowo, num_threads);

}