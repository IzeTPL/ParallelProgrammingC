#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <linux/sched.h>
#include "../pomiar_czasu/pomiar_czasu.h"

int zmienna_globalna = 0;

#define ROZMIAR_STOSU 1024*64

int funkcja_watku(void *argument) {

    for (int i = 0; i < 1000; i++)
        zmienna_globalna++;
    printf("Globalna: %d", zmienna_globalna);
    //int wynik;
    //wynik=execv("./program", NULL);
    //if(wynik==-1)
    //printf("Proces potomny nie wykonal programu\n");

    return 0;
}

int funkcja_watku2(int argument) {

    //int wynik;
    for (int i = 0; i < 1000; i++)
        argument++;
    printf("lokalna: %d", argument);
    //wynik=execv("./program", NULL);
    //if(wynik==-1)
    // printf("Proces potomny nie wykonal programu\n");

    return 0;
}

int main() {

    void *stos, *stos2;
    pid_t pid;
    pid_t pid2;
    int i, zmienna_lokalna = 0;

    stos = malloc(ROZMIAR_STOSU);
    if (stos == 0) {
        printf("Proces nadrzędny - blad alokacji stosu\n");
        exit(1);
    }

    stos2 = malloc(ROZMIAR_STOSU);
    if (stos2 == 0) {
        printf("Proces nadrzędny - blad alokacji stosu\n");
        exit(1);
    }

    inicjuj_czas();
    //for(i=0;i<1000;i++){

    pid = clone(&funkcja_watku, (void *) stos + ROZMIAR_STOSU,
                CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, 0);
    pid2 = clone(&funkcja_watku2, (void *) stos2 + ROZMIAR_STOSU,
                 CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, zmienna_lokalna);

    waitpid(pid, NULL, __WCLONE);
    waitpid(pid2, NULL, __WCLONE);

    //}
    printf("Globalna: %d", zmienna_globalna);
    printf("lokalna: %d", zmienna_lokalna);
    drukuj_czas();

    free(stos);
    free(stos2);
}
