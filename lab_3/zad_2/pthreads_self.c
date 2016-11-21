#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include <unistd.h>

int zmienna_wspolna = 0;

void *zadanie_watku(void *arg_wsk) {
	int i = *((int *) arg_wsk);

	printf("TID: %li\n", pthread_self());
	printf("ID: %d\n", i);

	return (NULL);
}

int main() {

	int thread_count = 10;
	pthread_t tid[thread_count];
	int tab_id[thread_count];
	for ( int i = 0; i < thread_count; ++i ) {

		tab_id[i] = i;
		pthread_create(&tid[i], NULL, zadanie_watku, &tab_id[i]);

	}

	for(int i = 0; i < thread_count; ++i) {
		pthread_join(tid[i], NULL);
	}

	pthread_exit(NULL);
}


