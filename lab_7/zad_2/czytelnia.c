#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"czytelnia.h"


/*** Implementacja procedur interfejsu ***/

int my_read_lock_lock(czytelnia_t *czytelnia_p) {
    pthread_mutex_lock(&czytelnia_p->mutex);
    if(czytelnia_p->liczba_pisz > 0 || czytelnia_p->liczba_czek_pisz != 0) {
        czytelnia_p->liczba_czek_czyt++;
        pthread_cond_wait(&czytelnia_p->czytelnicy, &czytelnia_p->mutex);
        czytelnia_p->liczba_czek_czyt--;
    }
    czytelnia_p->liczba_czyt++;
    pthread_mutex_unlock(&czytelnia_p->mutex);
    pthread_cond_broadcast(&czytelnia_p->czytelnicy);
}


int my_read_lock_unlock(czytelnia_t *czytelnia_p) {
    pthread_mutex_lock(&czytelnia_p->mutex);
    czytelnia_p->liczba_czyt--;
    if(czytelnia_p->liczba_czyt == 0) {
        pthread_mutex_unlock(&czytelnia_p->mutex);
        pthread_cond_signal(&czytelnia_p->pisarze);
    } else {
        pthread_mutex_unlock(&czytelnia_p->mutex);
    }
}


int my_write_lock_lock(czytelnia_t *czytelnia_p) {
    pthread_mutex_lock(&czytelnia_p->mutex);
    if(czytelnia_p->liczba_czyt+czytelnia_p->liczba_pisz > 0) {
        czytelnia_p->liczba_czek_pisz++;
        pthread_cond_wait(&czytelnia_p->pisarze, &czytelnia_p->mutex);
        czytelnia_p->liczba_czek_pisz++;
    }
    czytelnia_p->liczba_pisz++;
    pthread_mutex_unlock(&czytelnia_p->mutex);
}


int my_write_lock_unlock(czytelnia_t *czytelnia_p) {
    pthread_mutex_lock(&czytelnia_p->mutex);
    czytelnia_p->liczba_pisz--;
    if(czytelnia_p->liczba_czek_czyt != 0) {
        pthread_mutex_unlock(&czytelnia_p->mutex);
        pthread_cond_broadcast(&czytelnia_p->czytelnicy);
    } else {
        pthread_mutex_unlock(&czytelnia_p->mutex);
        pthread_cond_signal(&czytelnia_p->pisarze);
    }
}

void inicjuj(czytelnia_t *czytelnia_p) {
    czytelnia_p->liczba_pisz = 0;
    czytelnia_p->liczba_czyt = 0;
    czytelnia_p->liczba_czek_pisz = 0;
    czytelnia_p->liczba_czek_czyt = 0;
    czytelnia_p->czytelnicy = PTHREAD_COND_INITIALIZER;
    czytelnia_p->pisarze = PTHREAD_COND_INITIALIZER;
    czytelnia_p->mutex = PTHREAD_MUTEX_INITIALIZER;
}

void czytam(czytelnia_t *czytelnia_p) {
    usleep(rand() % 1000000);
}

void pisze(czytelnia_t *czytelnia_p) {
    usleep(rand() % 1000000);
}

