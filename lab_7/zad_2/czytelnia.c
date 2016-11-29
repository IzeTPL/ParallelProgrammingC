#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"czytelnia.h"


/*** Implementacja procedur interfejsu ***/

int my_read_lock_lock(czytelnia_t *czytelnia_p) {
    pthread_mutex_lock(&czytelnia_p->mutex);
    if(czytelnia_p->liczba_pisz > 0 || &czytelnia_p->pisarze != 0) {
        pthread_cond_wait(&czytelnia_p->czytelnicy, &czytelnia_p->mutex);
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
        pthread_cond_wait(&czytelnia_p->pisarze, &czytelnia_p->mutex);
    }
    czytelnia_p->liczba_pisz++;
    pthread_mutex_unlock(&czytelnia_p->mutex);
}


int my_write_lock_unlock(czytelnia_t *czytelnia_p) {
    pthread_mutex_lock(&czytelnia_p->mutex);
    czytelnia_p->liczba_pisz--;
    if(&czytelnia_p->czytelnicy != 0) {
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
    pthread_cond_init(&czytelnia_p->czytelnicy, NULL);
    pthread_cond_init(&czytelnia_p->pisarze, NULL);
    pthread_mutex_init(&czytelnia_p->mutex, NULL);
}

void czytam(czytelnia_t *czytelnia_p) {
    pthread_mutex_lock(&czytelnia_p->mutex);
    if(czytelnia_p->liczba_pisz > 0) pthread_exit(0);
    printf("Czytelnicy: %d\n", czytelnia_p->liczba_czyt);
    printf("Pisarze: %d\n", czytelnia_p->liczba_pisz);
    pthread_mutex_unlock(&czytelnia_p->mutex);
    usleep(rand() % 1000000);
}

void pisze(czytelnia_t *czytelnia_p) {
    pthread_mutex_lock(&czytelnia_p->mutex);
    if(czytelnia_p->liczba_czyt > 0 || czytelnia_p->liczba_pisz > 1) pthread_exit(0);
    printf("Czytelnicy: %d\n", czytelnia_p->liczba_czyt);
    printf("Pisarze: %d\n", czytelnia_p->liczba_pisz);
    pthread_mutex_unlock(&czytelnia_p->mutex);
    usleep(rand() % 1000000);
}

