# include <stdlib.h>
# include <stdio.h>
# include <math.h>

# include "mpi.h"

double pi = 0.0;

int main ( int argc, char *argv[] );

void count_pi_part(int p, int id, int start, int end) {
  double tmp = 0.0, a_n, b_n;
  int i;

  for (i = start; i >= end; i-=2) {
    a_n = (double)(1.0 / (2.0*i + 1.0));
    b_n = (double)(-1.0 / (2.0*(i+1) + 1.0));

    tmp += a_n + b_n;
  }

  MPI_Reduce(&tmp, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

}

int main ( int argc, char *argv[] )
{
  int id; // rank
  int p;  // size
  int n; //accuracy
    MPI_Status status;

  MPI_Init ( &argc, &argv );

  MPI_Comm_rank ( MPI_COMM_WORLD, &id );

  MPI_Comm_size ( MPI_COMM_WORLD, &p );

  if(id == 0) {

    scanf("%d", &n);

  }

      MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

   n = (int)floor(n / p);

  int end = id * ( n / p );
  if(end < 0) end = 0;

  int start = id + 1 * ( n / p );

  count_pi_part(p, id, start, end);

  if(id == 0) {
    //MPI_Barrier(MPI_COMM_WORLD);
    pi *= 4;
    printf("%d PI = %lf\n", id, pi);
    printf("Blad: %lf\n", (M_PI - pi));
  }

  MPI_Finalize ( );

  return;
}