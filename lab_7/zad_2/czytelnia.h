#ifndef _czytelnia_
#define _czytelnia_

/*** Definicje typow zmiennych ***/
typedef struct {
    int liczba_czyt;
    int liczba_pisz;
    int liczba_czek_czyt;
    int liczba_czek_pisz;
    pthread_cond_t czytelnicy;
    pthread_cond_t pisarze;
    pthread_mutex_t mutex;
} czytelnia_t;

/*** Deklaracje procedur interfejsu ***/
void inicjuj(czytelnia_t *czytelnia_p);

void czytam(czytelnia_t *czytelnia_p);

void pisze(czytelnia_t *czytelnia_p);

int my_read_lock_lock(czytelnia_t *czytelnia_p);

int my_read_lock_unlock(czytelnia_t *czytelnia_p);

int my_write_lock_lock(czytelnia_t *czytelnia_p);

int my_write_lock_unlock(czytelnia_t *czytelnia_p);

#endif
