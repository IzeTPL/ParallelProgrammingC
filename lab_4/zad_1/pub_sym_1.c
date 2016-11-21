#include<stdlib.h>
#include<stdio.h>
#include <time.h>
#include<pthread.h>
#include <unistd.h>


void *watek_klient(void *arg);

pthread_mutex_t mutex;

int l_kf, l_obieg, bylo_pite = 0;

main() {

	pthread_t *tab_klient;
	int *tab_klient_id;
	int l_kl, l_kr, i;

	pthread_mutex_init(&mutex, NULL);

	printf("\nLiczba klientow: ");
	scanf("%d", &l_kl);

	printf("\nLiczba kufli: ");
	scanf("%d", &l_kf);

	//printf("\nLiczba kranow: "); scanf("%d", &l_kr);
	l_kr = 1;
	l_obieg = l_kr;

	tab_klient = (pthread_t *) malloc(l_kl * sizeof(pthread_t));
	tab_klient_id = (int *) malloc(l_kl * sizeof(int));
	for (i = 0; i < l_kl; i++) tab_klient_id[i] = i;


	printf("\nOtwieramy pub (simple)!\n");
	printf("\nLiczba wolnych kufli %d\n", l_kf);

	for (i = 0; i < l_kl; i++) {
		pthread_create(&tab_klient[i], NULL, watek_klient, &tab_klient_id[i]);
	}
	for (i = 0; i < l_kl; i++) {
		pthread_join(tab_klient[i], NULL);
	}

	printf("\nLiczba wolnych kufli %d\n", l_kf);
	printf("\nByło pite %d\n", bylo_pite);
	printf("\nA powinno %d\n", 2*l_kl);
	printf("\nZamykamy pub!\n");
	pthread_mutex_destroy(&mutex);
	pthread_exit(NULL);

}


void *watek_klient(void *arg_wsk) {

	int moj_id = *((int *) arg_wsk);

	int i, j, kufel = 0, result;
	int ile_musze_wypic = 2;

	printf("\nKlient %d, wchodzę do pubu\n", moj_id);

	for (i = 0; i < ile_musze_wypic; i++) {

		printf("\nKlient %d, wybieram kufel\n", moj_id);
		while(kufel == 0) {

			pthread_mutex_lock(&mutex);
			if (l_kf > 0) {
				l_kf--;
				kufel = 1;
				result = 1;
			} else if (l_kf == 0) {
				printf("\nKlient %d, brak wolnych kufli, czekam\n", moj_id);
				result = 0;
			} else if (l_kf < 0 || l_kf >= l_obieg) {
				printf("co kurwa??? %d", l_kf);
			}
			pthread_mutex_unlock(&mutex);

			if(result == 0) sleep(1);
		}

		j = 0;
		printf("\nKlient %d, nalewam z kranu %d\n", moj_id, j);
		usleep(300);

		printf("\nKlient %d, pije\n", moj_id);
		nanosleep((struct timespec[]) {{0, 500000000L}}, NULL);
		pthread_mutex_lock(&mutex);
		bylo_pite++;
		pthread_mutex_unlock(&mutex);

		printf("\nKlient %d, odkladam kufel\n", moj_id);
		pthread_mutex_lock(&mutex);
		l_kf++;
		kufel = 0;
		pthread_mutex_unlock(&mutex);

	}

	printf("\nKlient %d, wychodzę z pubu\n", moj_id);

	return (NULL);
} 


