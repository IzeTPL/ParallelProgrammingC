#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<math.h>
#include "../../pomiar_czasu/pomiar_czasu.h"

#define ROZMIAR 10000000
#define LICZBA_W 8
#define A 0.0
#define B M_PI

pthread_mutex_t muteks;
pthread_t watki[LICZBA_W];
double global_array_of_local_sums[LICZBA_W];

struct params {

	double a;
	double b;
	int N;

};

double a = A;
double b = B;

double calka(struct params);

void *calka_rownolegla(void *arg_wsk);

void *calka_dziedzina(void *arg_wsk);

double f(double);

double suma = 0;

int main(int argc, char *argv[]) {

	struct params p;
	double t1,t2,t3;
	struct params params[LICZBA_W];
	p.N = ROZMIAR;
	p.a = 0;
	p.b = M_PI;

	t1 = czas_zegara();
	printf("Wynik sekwencyjny: %.12lf\n", calka(p));
	t1 = czas_zegara() - t1;
	printf("Czas obliczen = %lf\n", t1);

	pthread_mutex_init( &muteks, NULL);

	int i;

	int indeksy[LICZBA_W];
	for (i = 0; i < LICZBA_W; i++) indeksy[i] = i;

	t1 = czas_zegara();
	for (i = 0; i < LICZBA_W; i++) {
		global_array_of_local_sums[i] = 0.0;
		pthread_create(&watki[i], NULL, calka_rownolegla, (void *) &indeksy[i]);
	}

	for (i = 0; i < LICZBA_W; i++) {
		pthread_join(watki[i], NULL);
		suma += global_array_of_local_sums[i];
	}

	t1 = czas_zegara() - t1;
	printf("Czas obliczen = %lf\n", t1);

	printf("wynik zrown = %.12lf\n\n", suma);

	suma = 0.0;
	double dx = (B - A)/ROZMIAR;
	int n_global = ROZMIAR /LICZBA_W;

	t1 = czas_zegara();
	for (i = 0; i < LICZBA_W; i++) {

		if((ROZMIAR % LICZBA_W) != 0) {

			params[i].N = n_global + 1;

		} else {

			params[i].N = n_global;

		}

		int warunek = params[i].N * (i + 1);

		if(warunek > ROZMIAR) {
			params[i].N -= (warunek  - ROZMIAR);
		}

		b = a + (params[i].N*dx);
		params[i].a = a;
		params[i].b = b;
		a = b;
		pthread_create(&watki[i], NULL, calka_dziedzina, (void *) &params[i]);

	}

	for (i = 0; i < LICZBA_W; i++) {
		pthread_join(watki[i], NULL);
	}

	t1 = czas_zegara() - t1;
	printf("Czas obliczen = %lf\n", t1);

	printf("wynik dekomp = %.12lf\n", suma);
}

void *calka_rownolegla(void *arg_wsk) {

	int i, n_local, n_pocz, n_koniec, moj_id;
	double x1,x2,dx;

	moj_id = *((int *) arg_wsk);

	dx = (B - A) / ROZMIAR;

	if((ROZMIAR % LICZBA_W) != 0) {

		n_local = ROZMIAR / LICZBA_W + 1;

	} else {

		n_local = ROZMIAR / LICZBA_W;

	}

	n_pocz = n_local * moj_id;
	n_koniec = n_local * (moj_id + 1);
	if(n_koniec > ROZMIAR) n_koniec = ROZMIAR;

	printf("a: %lf b: %lf N: %d\n", n_pocz * dx, n_koniec * dx, (n_koniec - n_pocz) );

	for (i = n_pocz; i < n_koniec; i++) {
		x1 = i * dx;
		global_array_of_local_sums[moj_id]+=0.5*(f(x1)+f(x1 + dx))*dx;
	}

	pthread_exit((void *) 0);

}

void *calka_dziedzina(void *arg_wsk) {

	struct params p;
	p = *(struct params*) arg_wsk;


	int i;
	double dx, x1, x2, c;

	x1 = p.a;
	c=0.0;
	dx = (p.b-p.a)/p.N;

	printf("a: %lf b: %lf N: %d\n", p.a, p.b, p.N);

	for(i=0;i<p.N;i++){
		x2=x1+dx;
		c+=0.5*(f(x1)+f(x2))*dx;
		x1=x2;
	}

	pthread_mutex_lock( &muteks );
	suma += c;
	pthread_mutex_unlock( &muteks );

	pthread_exit( (void *)0);

}

double calka(struct params p) {

	int i;
	double dx, x1, x2, c;

	x1 = p.a;
	c=0.0;
	dx = (p.b-p.a)/p.N;

	for(i=0;i<p.N;i++){
		x2=x1+dx;
		c+=0.5*(f(x1)+f(x2))*dx;
		x1=x2;
	}

	return c;
}

double f(double x) {

	return sin(x);

}