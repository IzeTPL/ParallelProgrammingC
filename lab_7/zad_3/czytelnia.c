#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"czytelnia.h"


/*** Implementacja procedur interfejsu ***/

int my_read_lock_lock(czytelnia_t *czytelnia_p) {

    pthread_rwlock_rdlock(&czytelnia_p->rwlock);
    czytelnia_p->liczba_czyt++;

}


int my_read_lock_unlock(czytelnia_t *czytelnia_p) {

    czytelnia_p->liczba_czyt--;
    pthread_rwlock_unlock(&czytelnia_p->rwlock);

}


int my_write_lock_lock(czytelnia_t *czytelnia_p) {

    pthread_rwlock_wrlock(&czytelnia_p->rwlock);
    czytelnia_p->liczba_pisz++;

}


int my_write_lock_unlock(czytelnia_t *czytelnia_p) {

    czytelnia_p->liczba_pisz--;
    pthread_rwlock_unlock(&czytelnia_p->rwlock);

}

void inicjuj(czytelnia_t *czytelnia_p) {
    czytelnia_p->liczba_pisz = 0;
    czytelnia_p->liczba_czyt = 0;
    czytelnia_p->liczba_czek_pisz = 0;
    czytelnia_p->liczba_czek_czyt = 0;
    pthread_cond_init(&czytelnia_p->czytelnicy, NULL);
    pthread_cond_init(&czytelnia_p->pisarze, NULL);
    pthread_rwlock_init(&czytelnia_p->rwlock, NULL);
    pthread_mutex_init(&czytelnia_p->mutex, NULL);
}

void czytam(czytelnia_t *czytelnia_p) {
    pthread_mutex_lock(&czytelnia_p->mutex);
    printf("Czytelnicy: %d\n", czytelnia_p->liczba_czyt);
    printf("Pisarze: %d\n", czytelnia_p->liczba_pisz);
    pthread_mutex_unlock(&czytelnia_p->mutex);
    usleep(rand() % 1000000);
}

void pisze(czytelnia_t *czytelnia_p) {
    pthread_mutex_lock(&czytelnia_p->mutex);
    printf("Czytelnicy: %d\n", czytelnia_p->liczba_czyt);
    printf("Pisarze: %d\n", czytelnia_p->liczba_pisz);
    pthread_mutex_unlock(&czytelnia_p->mutex);
    usleep(rand() % 1000000);
}

