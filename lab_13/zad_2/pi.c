# include <stdlib.h>
# include <stdio.h>
# include <math.h>

# include "mpi.h"

int main ( int argc, char *argv[] );

void count_pi_part(int p, int id, long int start, long int end) {
  double tmp = 0.0, a_n;
  long int i;

  for (i = start; i >= end; i--) {
    if (i % 2 == 0) {
      a_n = (double)(1.0 / (2.0*i + 1.0));
    } else {
      a_n = (double)(-1.0 / (2.0*i + 1.0));
    }

    tmp = tmp + a_n;
  }

  MPI_Send(&tmp, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

}

int main ( int argc, char *argv[] )
{
  int id; // rank
  int p;  // size
  int n; //accuracy
    double pi;
    MPI_Status status;

  MPI_Init ( &argc, &argv );

  MPI_Comm_rank ( MPI_COMM_WORLD, &id );

  MPI_Comm_size ( MPI_COMM_WORLD, &p );

  if(id == 0) {

    scanf("Podaj dokladnosc: %d", &n);
      //MPI_Gather(&pi, 1, MPI_DOUBLE, )

  } else {

      MPI_Bcast(&n, 1, MPI_LONG_INT, 0, MPI_COMM_WORLD);

  }

  long int end = id * ( n / p );

  long int start = id + 1 * ( n / p );

  count_pi_part(p, id, start, end);

  //MPI_Barrier( MPI_COMM_WORLD );

  MPI_Finalize ( );

  return;
}