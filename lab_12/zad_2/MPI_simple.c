#include <stdlib.h>
#include<stdio.h>
#include<math.h>
#include <unistd.h>

#include "mpi.h"


int main( int argc, char** argv ){ 
  
  int rank, ranksent, size, source, dest, tag, i; 
  MPI_Status status;
  char hostname[255], hostnamesend[255];
  
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank ); 
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  
  if(size>1){
	tag = 0;
 
	if( rank != 0 ) {
		MPI_Recv( &hostnamesend, 255, MPI_CHAR, rank - 1, tag , MPI_COMM_WORLD, &status );
		printf("jestem %d, Dane od procesu o randze (i=%d): %s\n", rank, status.MPI_SOURCE, hostnamesend );
	} 
	if(rank == (size - 1) ){
		gethostname(hostname, 255); 
		MPI_Send( &hostname, 255, MPI_CHAR, 0, tag, MPI_COMM_WORLD );
		printf("jestem %d, Wysyłam do procesu o randze (i=%d):\n", rank, 0 );
	} else if (rank == 0){
		gethostname(hostname, 255); 
		MPI_Send( &hostname, 255, MPI_CHAR, rank + 1, tag, MPI_COMM_WORLD );
		printf("jestem %d, Wysyłam do procesu o randze (i=%d):\n", rank, rank + 1 );
		MPI_Recv( &hostnamesend, 255, MPI_CHAR, size - 1, tag , MPI_COMM_WORLD, &status );
		printf("jestem %d, Dane od procesu o randze (i=%d): %s (koniec obiegu)\n", rank, status.MPI_SOURCE,  hostnamesend );
	} else {
		
		gethostname(hostname, 255); 
		MPI_Send( &hostname, 255, MPI_CHAR, rank + 1, tag, MPI_COMM_WORLD );
		printf("jestem %d, Wysyłam do procesu o randze (i=%d):\n", rank, rank + 1 );

	}
      

  } else{
	printf("Pojedynczy proces o hostname: %s (brak komunikatów)\n", hostname);
  }

  
  MPI_Finalize(); 
  
  return(0);

}

