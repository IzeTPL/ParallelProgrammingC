#include <stdlib.h>
#include<stdio.h>
#include<math.h>

#include "mpi.h"

struct rekord {
    double skalar;
    char znak;
    float wektor[3];
};


int main(int argc, char **argv) {

    int rozm, rozm_pakietu, pozycja, pozycja_odb;
    void *bufor;


    int rank, ranksent, size, source, dest, tag, i;
    MPI_Status status;
    char hostname[255], hostnamesend[255];
    struct rekord baza;

    baza.skalar = 1.98;
    baza.znak = 'z';
    baza.wektor[0] = 4.1;
    baza.wektor[1] = 2.1;
    baza.wektor[2] = 5.1;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    rozm_pakietu = 0;

    MPI_Pack_size(1, MPI_DOUBLE, MPI_COMM_WORLD, &rozm);
    rozm_pakietu = rozm;
    MPI_Pack_size(1, MPI_CHAR, MPI_COMM_WORLD, &rozm);
    rozm_pakietu += rozm;
    MPI_Pack_size(3, MPI_FLOAT, MPI_COMM_WORLD, &rozm);
    rozm_pakietu += rozm;
    bufor = (void *) malloc(rozm_pakietu);
    pozycja = 0;

    if (size > 1) {
        tag = 0;

        if (rank != 0) {
            MPI_Recv(bufor, rozm_pakietu, MPI_PACKED, rank - 1, tag, MPI_COMM_WORLD, &status);
            pozycja = 0;
            MPI_Unpack(bufor, rozm_pakietu, &pozycja, &baza.skalar, 1, MPI_DOUBLE, MPI_COMM_WORLD);
            MPI_Unpack(bufor, rozm_pakietu, &pozycja, &baza.znak, 1, MPI_CHAR, MPI_COMM_WORLD);
            MPI_Unpack(bufor, rozm_pakietu, &pozycja, &baza.wektor, 3, MPI_FLOAT, MPI_COMM_WORLD);
            printf("jestem %d, Dane od procesu o randze (i=%d): \n", rank, status.MPI_SOURCE);
            printf("skalar: %lf, znak: %c, wektor: %lf\n\n", baza.skalar, baza.znak, baza.wektor[0]);
        }
        if (rank == (size - 1)) {
            pozycja = 0;
            MPI_Pack(&baza.skalar, 1, MPI_DOUBLE, bufor, rozm_pakietu, &pozycja, MPI_COMM_WORLD);
            MPI_Pack(&baza.znak, 1, MPI_CHAR, bufor, rozm_pakietu, &pozycja, MPI_COMM_WORLD);
            MPI_Pack(&baza.wektor[0], 3, MPI_FLOAT, bufor, rozm_pakietu, &pozycja, MPI_COMM_WORLD);
            MPI_Send(bufor, pozycja, MPI_PACKED, 0, tag, MPI_COMM_WORLD);
            printf("jestem %d, Wysyłam do procesu o randze (i=%d):\n", rank, 0);
            printf("skalar: %lf, znak: %c, wektor: %lf\n\n", baza.skalar, baza.znak, baza.wektor[0]);
        } else if (rank == 0) {

            pozycja = 0;
            MPI_Pack(&baza.skalar, 1, MPI_DOUBLE, bufor, rozm_pakietu, &pozycja, MPI_COMM_WORLD);
            MPI_Pack(&baza.znak, 1, MPI_CHAR, bufor, rozm_pakietu, &pozycja, MPI_COMM_WORLD);
            MPI_Pack(&baza.wektor[0], 3, MPI_FLOAT, bufor, rozm_pakietu, &pozycja, MPI_COMM_WORLD);
            MPI_Send(bufor, pozycja, MPI_PACKED, rank + 1, tag, MPI_COMM_WORLD);
            printf("jestem %d, Wysyłam do procesu o randze (i=%d):\n", rank, rank + 1);
            printf("skalar: %lf, znak: %c, wektor: %lf\n\n", baza.skalar, baza.znak, baza.wektor[0]);

            MPI_Recv(bufor, rozm_pakietu, MPI_PACKED, size - 1, tag, MPI_COMM_WORLD, &status);
            pozycja = 0;
            MPI_Unpack(bufor, rozm_pakietu, &pozycja, &baza.skalar, 1, MPI_DOUBLE, MPI_COMM_WORLD);
            MPI_Unpack(bufor, rozm_pakietu, &pozycja, &baza.znak, 1, MPI_CHAR, MPI_COMM_WORLD);
            MPI_Unpack(bufor, rozm_pakietu, &pozycja, &baza.wektor[0], 3, MPI_FLOAT, MPI_COMM_WORLD);
            printf("jestem %d, Dane od procesu o randze (i=%d): \n", rank, status.MPI_SOURCE);
            printf("skalar: %lf, znak: %c, wektor: %lf\n\n", baza.skalar, baza.znak, baza.wektor[0]);
        } else {

            pozycja = 0;
            MPI_Pack(&baza.skalar, 1, MPI_DOUBLE, bufor, rozm_pakietu, &pozycja, MPI_COMM_WORLD);
            MPI_Pack(&baza.znak, 1, MPI_CHAR, bufor, rozm_pakietu, &pozycja, MPI_COMM_WORLD);
            MPI_Pack(&baza.wektor[0], 3, MPI_FLOAT, bufor, rozm_pakietu, &pozycja, MPI_COMM_WORLD);
            MPI_Send(bufor, pozycja, MPI_PACKED, rank + 1, tag, MPI_COMM_WORLD);
            printf("jestem %d, Wysyłam do procesu o randze (i=%d):\n", rank, 0);
            printf("skalar: %lf, znak: %c, wektor: %lf\n\n", baza.skalar, baza.znak, baza.wektor[0]);

        }


    } else {
        printf("Pojedynczy proces o hostname: %s (brak komunikatów)\n", hostname);
    }


    MPI_Finalize();

    return (0);

}

