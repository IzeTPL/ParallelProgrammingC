# include <stdlib.h>
# include <stdio.h>
# include <math.h>
#include <unistd.h>

# include "mpi.h"

int main ( int argc, char *argv[] );
void heat_part ( int n, int p, int id, double x_min, double x_max );

/******************************************************************************/

int main ( int argc, char *argv[] )
{
  double a = 0.0; // lewy brzeg przedzialu
  double b = 1.0; // prawy brzeg przedzialu
  int i;  
  int id; // rank
  int n;  // liczba punktow dla kazdego wezla
  int p;  // size
  double x_max;
  double x_min;

  MPI_Init ( &argc, &argv );

  MPI_Comm_rank ( MPI_COMM_WORLD, &id );

  MPI_Comm_size ( MPI_COMM_WORLD, &p );

  n = 12; // liczba punktow dla kazdego wezla 
  i = 0;  // poczatkowa chwila czasu

  // wspolrzedna lewego punktu dla wezla id
  x_min = ( ( double )( p * n + 1 - id * n - i ) * a   
          + ( double )(             id * n + i ) * b ) 
          / ( double ) ( p * n + 1              );

  i = n + 1;

  // wspolrzedna prawego punktu dla wezla id
  x_max = ( ( double )( p * n + 1 - id * n - i ) * a   
          + ( double )(             id * n + i ) * b ) 
          / ( double )( p * n + 1              );

  heat_part ( n, p, id, x_min, x_max ); // obliczenia dla pojedynczego wezla

  MPI_Barrier( MPI_COMM_WORLD );

  MPI_Finalize ( );

  return;
}

/******************************************************************************/
// obliczenia dla pojedynczego wezla - pojedynczego podobszaru
/******************************************************************************/
void heat_part ( int n, int p, int id, double x_min, double x_max )
{
  double cfl;
  double *h;
  double *h_new;
  int i;
  int ierr;
  int j;
  int j_max;
  int j_min;
  double k;
  MPI_Status status;
  double t;
  double t_del;
  double t_max;
  double t_min;
  int tag;
  double wtime;
  double *x;
  double x_del;

  h = ( double * ) malloc ( ( n + 2 ) * sizeof ( double ) ); // rozwiazanie dla t_i
  h_new = ( double * ) malloc ( ( n + 2 ) * sizeof ( double ) ); // rozwiazanie dla t_i+1
  x = ( double * ) malloc ( ( n + 2 ) * sizeof ( double ) ); // wspolrzedne punktow

  k = 0.002 / ( double ) p; // przewodniosc cieplna

  j_min = 0; // indeksy krokow czasowych - min i max
  j_max = 100;
  t_min = 0.0; // chwile czasu - min i max
  t_max = 10.0;
  t_del = ( t_max - t_min ) / ( double ) ( j_max - j_min ); // krok czasowy Delta t

  x_del = ( x_max - x_min ) / ( double ) ( n + 1 ); // odstep miedzy punktami
  for ( i = 0; i <= n + 1; i++ )
  {
    x[i] = ( ( double ) (         i ) * x_max   
           + ( double ) ( n + 1 - i ) * x_min ) 
           / ( double ) ( n + 1     );
  }

  // ustawienie warunku poczatkowego
  for ( i = 0; i <= n + 1; i++ )
  {
    h[i] = 95.0;
  }

  wtime = MPI_Wtime ( ); //poczatek pomiaru czasu

  for ( j = 1; j <= j_max; j++ )
  {

    // wymiana informacji z wezlami sasiednimi
    tag = 1;

    if ( id < p - 1 )
    {
      MPI_Send ( &h[n], 1, MPI_DOUBLE, id+1, tag, MPI_COMM_WORLD );
    }

    if ( 0 < id )
    {
      MPI_Recv ( &h[0], 1, MPI_DOUBLE, id-1, tag, MPI_COMM_WORLD, &status );
    }

    tag = 2;

    if ( 0 < id )
    {
      MPI_Send ( &h[1], 1, MPI_DOUBLE, id-1, tag, MPI_COMM_WORLD );
    }

    if ( id < p - 1 )
    {
      MPI_Recv ( &h[n+1], 1, MPI_DOUBLE, id+1, tag, MPI_COMM_WORLD, &status );
    }


    // implementacja wzoru roznicowego
    for ( i = 1; i <= n; i++ )
    {
      h_new[i] = h[i] + t_del * ( 
        k * ( h[i-1] - 2.0 * h[i] + h[i+1] ) / x_del / x_del 
        + 2.0 * sin ( x[i] * t ) );
    }

    // nowa chwila czasu
    t = ( ( double ) ( j - j_min ) * t_max   
            + ( double ) ( j_max - j         ) * t_min ) 
            / ( double ) ( j_max     - j_min );

    // przygotowanie do nastepnego kroku czasowego
    for ( i = 1; i < n + 1; i++ )
    {
      h[i] = h_new[i];
    }

    // warunek brzegowy
    if ( 0 == id ) h[0] = 100.0 + 10.0 * sin ( t );
    if ( id == p - 1 ) h[n+1] = 75;

  }

  // koncowa wymiana informacji z wezlami sasiednimi
  tag = 11;
  
  if ( id < p - 1 ) {
    MPI_Send ( &h[n], 1, MPI_DOUBLE, id+1, tag, MPI_COMM_WORLD );
  }
  
  if ( 0 < id ) {
    MPI_Recv ( &h[0], 1, MPI_DOUBLE, id-1, tag, MPI_COMM_WORLD, &status );
  }
  
  tag = 12;

  if ( 0 < id )
  {
    MPI_Send ( &h[1], 1, MPI_DOUBLE, id-1, tag, MPI_COMM_WORLD );
  }

  if ( id < p - 1 )
  {
    MPI_Recv ( &h[n+1], 1, MPI_DOUBLE, id+1, tag, MPI_COMM_WORLD, &status );
  }
  
  wtime = MPI_Wtime ( ) - wtime;

  int idcomm = id;

  if (id == 0){
    printf ( "\n" );
    printf ( "  Wall clock elapsed seconds = %f\n", wtime );

    printf ( "%2d  T= %f\n", id, t );
    printf ( "%2d  X= ", id );
    for ( i = 0; i <= n + 1; i++ )
    {
      printf ( "%7.2f", x[i] );
    }
    printf ( "\n" );
    printf ( "%2d  H= ", id );
    for ( i = 0; i <= n + 1; i++ )
    {
      printf ( "%7.2f", h[i] );
    }
    printf ( "\n" );

    free ( h );
    free ( h_new );
    free ( x );

    MPI_Send( &idcomm, 1, MPI_INT, id + 1, 0, MPI_COMM_WORLD );

    //printf("jestem %d, Wysyłam do procesu o randze (i=%d):\n", id, id + 1 );
  } else {

    MPI_Recv( &idcomm, 1, MPI_INT, id - 1, 0 , MPI_COMM_WORLD, &status );

    printf ( "%2d  T= %f\n", id, t );
    printf ( "%2d  X= ", id );
    for ( i = 0; i <= n + 1; i++ )
    {
      printf ( "%7.2f", x[i] );
    }
    printf ( "\n" );
    printf ( "%2d  H= ", id );
    for ( i = 0; i <= n + 1; i++ )
    {
      printf ( "%7.2f", h[i] );
    }
    printf ( "\n" );

    free ( h );
    free ( h_new );
    free ( x );

    //printf("jestem %d, Dane od procesu o randze (i=%d)\n", id, status.MPI_SOURCE );
    if(id != p - 1) {
      MPI_Send(&idcomm, 1, MPI_INT, id + 1, 0, MPI_COMM_WORLD);
      //printf("jestem %d, Wysyłam do procesu o randze (i=%d):\n", id, id + 1);
    }

  }

  // wydruk wyniku


  return;
}
