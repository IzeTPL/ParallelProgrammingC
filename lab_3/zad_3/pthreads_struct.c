#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include <unistd.h>

typedef struct punkt_3D punkt_3D;

struct punkt_3D{

	int x, y, z;

};

void *zadanie_watku(void *arg_wsk) {

	punkt_3D *punkt_3D_main = (punkt_3D *) arg_wsk;
	punkt_3D punkt3D_lokalny = *((punkt_3D *) arg_wsk);

	printf("Struktura globalna w procedurze wątku %lu przed zmianami:\n", pthread_self());
	printf("x:%d\n",punkt_3D_main->x);
	printf("y:%d\n",punkt_3D_main->y);
	printf("z:%d\n",punkt_3D_main->z);

	punkt_3D_main->x += 1;
	punkt_3D_main->y += 1;
	punkt_3D_main->z += 1;

	printf("Struktura globalna w procedurze wątku %lu po zmianach:\n", pthread_self());
	printf("x:%d\n",punkt_3D_main->x);
	printf("y:%d\n",punkt_3D_main->y);
	printf("z:%d\n",punkt_3D_main->z);

	printf("Struktura lokalna w procedurze wątku %lu przed zmianami\n", pthread_self());
	printf("x:%d\n",punkt3D_lokalny.x);
	printf("y:%d\n",punkt3D_lokalny.y);
	printf("z:%d\n",punkt3D_lokalny.z);

	punkt3D_lokalny.x += 2;
	punkt3D_lokalny.y += 2;
	punkt3D_lokalny.z += 2;

	printf("Struktura lokalna w procedurze wątku %lu po zmianach:\n", pthread_self());
	printf("x:%d\n",punkt3D_lokalny.x);
	printf("y:%d\n",punkt3D_lokalny.y);
	printf("z:%d\n",punkt3D_lokalny.z);

	return (NULL);

}

int main() {

	punkt_3D punkt = {1,2,3};

	int zmienna_wspolna = 0;
	int thread_count = 2;
	pthread_t tid[thread_count];

	printf("Struktura globalna w main przed zmianami:\n");
	printf("x:%d\n",punkt.x);
	printf("y:%d\n",punkt.y);
	printf("z:%d\n",punkt.z);

	for ( int i = 0; i < thread_count; ++i ) {

		pthread_create(&tid[i], NULL, zadanie_watku, &punkt);

	}

	for(int i = 0; i < thread_count; ++i) {
		pthread_join(tid[i], NULL);
	}

	printf("Struktura globalna w main po zmianach:\n");
	printf("x:%d\n",punkt.x);
	printf("y:%d\n",punkt.y);
	printf("z:%d\n",punkt.z);

}


