#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define WYMIAR 10000
#define ROZMIAR WYMIAR*WYMIAR

void mat_vec_row(double* a, double* x, double* y, int n);
void mat_vec_col(double* a, double* x, double* y, int n);

main () {
  static double a[ROZMIAR], aa[ROZMIAR], x[WYMIAR], y[WYMIAR], z[WYMIAR];
  int n, i, j;
  const int ione = 1;
  const double done = 1.0;
  const double dzero = 0.0;
  double t1, t2;


  for (i = 0; i < ROZMIAR; i++) a[i] = 1.0001 * i;
  for (i = 0; i < ROZMIAR; i++) aa[i] = 1.0001 * i;
  for (i = 0; i < WYMIAR; i++) x[i] = 1.0 * (WYMIAR - i);

  n = WYMIAR;
  double nr_oper = (n * (n * 2.0));

  printf("ROW MAJOR\n");

  t1 = omp_get_wtime();
  mat_vec_row(a, x, y, n);
  t1 = omp_get_wtime() - t1;
  printf("time for one multiplication: %lf, Gflop/s: %lf, GB/s> %lf\n",
         t1, 2.0e-9 * ROZMIAR / t1, 8.0e-9 * ROZMIAR / t1);

  // test
  printf("TEST ROW MAJOR\n");
double local;
  t1 = omp_get_wtime();
#pragma omp parallel for default(none) private(i,j,local) firstprivate(n) shared(a,x,z)
  for (i = 0; i < n; i++) {
    z[i] = 0.0;
    local = 0.0;
    for (j = 0; j < n; j++) {
      local += a[n * i + j] * x[j];
    }
#pragma omp atomic
    z[i]+=local;
  }

  t1=omp_get_wtime()-t1;
  printf("time for one multiplication: %lf, Gflop/s: %lf, GB/s> %lf\n",  
	 t1, 2.0e-9*ROZMIAR/t1, 8.0e-9*ROZMIAR/t1);

  /* printf("czas dostepu min %lf, max %lf\n", */
  /* 	 1024*1024*1024*t1/nr_oper, 1024*1024*1024*t1/(nr_oper/2)); */
  /* printf("GFLOPS = %lf, GB/s - min = %lf, max = %lf\n\n",  */
  /* 	 nr_oper/t1/1024/1024/1024, 4*nr_oper/t1/1024/1024/1024, */
  /* 	 8*nr_oper/t1/1024/1024/1024); */

  for(i=0;i<WYMIAR;i++){
    if(fabs(y[i]-z[i])>1.e-9*z[i]) printf("Blad!\n");
  }


  printf("COLUMN MAJOR\n");

  for(i=0;i<n;i++) y[i]=0.0;
  t1=omp_get_wtime();
  mat_vec_col(a,x,y,n);
  t1=omp_get_wtime()-t1;
  printf("time for one multiplication: %lf, Gflop/s: %lf, GB/s> %lf\n",  
	 t1, 2.0e-9*ROZMIAR/t1, 8.0e-9*ROZMIAR/t1);

  // test
  printf("TEST COLUMN MAJOR\n");

  double local_2;

  t1=omp_get_wtime();
#pragma omp parallel for shared(z) private(i)
  for(i=0;i<n;i++) z[i]=0.0;
#pragma omp parallel for default(none) private(i,j,local) firstprivate(n) shared(a,x,z)
  for(j=0;j<n;j++){
    for(i=0;i<n;i++){
#pragma omp atomic
      z[i]+=a[i+j*n]*x[j];
    }
  }
  t1=omp_get_wtime()-t1;
  printf("time for one multiplication: %lf, Gflop/s: %lf, GB/s> %lf\n",  
	 t1, 2.0e-9*ROZMIAR/t1, 8.0e-9*ROZMIAR/t1);

  for(i=0;i<WYMIAR;i++){
    if(fabs(y[i]-z[i])>1.e-9*z[i]) printf("Blad!\n");
  }

  printf("TEST 2D\n");

  mat_vec_row(a, x, y, n);

  double local_2d;
  t1 = omp_get_wtime();
#pragma omp parallel for default(none) private(i,j,local_2d) firstprivate(n) shared(a,x,z)
  for (i = 0; i < n; i++) {
    z[i] = 0.0;
    local_2d = 0.0;
#pragma omp parallel for default(none) firstprivate(i, n) shared(a,x,z,local_2d) private(j)
    for (j = 0; j < n; j++) {
#pragma omp atomic
      local_2d += a[n * i + j] * x[j];
    }
#pragma omp atomic
    z[i]+=local_2d;
  }

  t1=omp_get_wtime()-t1;
  printf("time for one multiplication: %lf, Gflop/s: %lf, GB/s> %lf\n",
         t1, 2.0e-9*ROZMIAR/t1, 8.0e-9*ROZMIAR/t1);

  for(i=0;i<WYMIAR;i++){
    if(fabs(y[i]-z[i])>1.e-9*z[i]) printf("Blad!\n");
  }

}
