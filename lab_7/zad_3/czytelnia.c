#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"czytelnia.h"


/*** Implementacja procedur interfejsu ***/

int my_read_lock_lock(czytelnia_t *czytelnia_p) {

    pthread_rwlock_rdlock(&czytelnia_p->rwlock);

    if(czytelnia_p->liczba_pisz > 0 || czytelnia_p->liczba_czek_pisz != 0) {

        pthread_rwlock_wrlock(&czytelnia_p->rwlock);

        czytelnia_p->liczba_czek_czyt++;
        pthread_cond_wait(&czytelnia_p->czytelnicy, &czytelnia_p->rwlock);
        czytelnia_p->liczba_czek_czyt--;

        pthread_rwlock_unlock(&czytelnia_p->rwlock);
    }

    pthread_rwlock_wrlock(&czytelnia_p->rwlock);
    czytelnia_p->liczba_czyt++;
    pthread_rwlock_unlock(&czytelnia_p->rwlock);

    pthread_rwlock_unlock(&czytelnia_p->rwlock);

    pthread_cond_broadcast(&czytelnia_p->czytelnicy);
}


int my_read_lock_unlock(czytelnia_t *czytelnia_p) {

    pthread_rwlock_wrlock(&czytelnia_p->rwlock);
    czytelnia_p->liczba_czyt--;
    pthread_rwlock_unlock(&czytelnia_p->rwlock);

    pthread_rwlock_rdlock(&czytelnia_p->rwlock);

    if(czytelnia_p->liczba_czyt == 0) {

        pthread_cond_signal(&czytelnia_p->pisarze);
        pthread_rwlock_unlock(&czytelnia_p->rwlock);

    } else {
        pthread_rwlock_unlock(&czytelnia_p->rwlock);
    }
}


int my_write_lock_lock(czytelnia_t *czytelnia_p) {

    pthread_rwlock_rdlock(&czytelnia_p->rwlock);

    if(czytelnia_p->liczba_czyt + czytelnia_p->liczba_pisz > 0) {

        pthread_rwlock_wrlock(&czytelnia_p->rwlock);

        czytelnia_p->liczba_czek_pisz++;
        pthread_cond_wait(&czytelnia_p->pisarze, &czytelnia_p->rwlock);
        czytelnia_p->liczba_czek_pisz++;

        pthread_rwlock_unlock(&czytelnia_p->rwlock);
    }

    pthread_rwlock_wrlock(&czytelnia_p->rwlock);
    czytelnia_p->liczba_pisz++;
    pthread_rwlock_unlock(&czytelnia_p->rwlock);

    pthread_rwlock_unlock(&czytelnia_p->rwlock);

}


int my_write_lock_unlock(czytelnia_t *czytelnia_p) {

    pthread_rwlock_wrlock(&czytelnia_p->rwlock);
    czytelnia_p->liczba_pisz--;
    pthread_rwlock_unlock(&czytelnia_p->rwlock);

    pthread_rwlock_rdlock(&czytelnia_p->rwlock);

    if(czytelnia_p->liczba_czek_czyt != 0) {
        pthread_rwlock_unlock(&czytelnia_p->rwlock);
        pthread_cond_broadcast(&czytelnia_p->czytelnicy);
    } else {
        pthread_rwlock_unlock(&czytelnia_p->rwlock);
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
    czytelnia_p->rwlock = PTHREAD_RWLOCK_INITIALIZER;
}

void czytam(czytelnia_t *czytelnia_p) {
    usleep(rand() % 1000000);
}

void pisze(czytelnia_t *czytelnia_p) {
    usleep(rand() % 1000000);
}

