#include <stdlib.h>
#include<stdio.h>
#include<math.h>

#include "mpi.h"

int main( int argc, char** argv ){ 
  
  int rank, ranksent, size, source, dest, tag, i; 
  MPI_Status status;
  char hostname[255], hostnamesend[255];
  
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank ); 
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  
  if(size>1){
    //wszystkie pozostale
    if( rank != 0 ){ dest=0; tag=0; 

	gethostname(hostname, 255);      
      MPI_Send( &hostname, 255, MPI_CHAR, dest, tag, MPI_COMM_WORLD );
      
    } else { //zerowy proces
      
      for( i=1; i<size; i++ ) { 
	
	MPI_Recv( &hostnamesend, 255, MPI_CHAR, MPI_ANY_SOURCE, 
		  MPI_ANY_TAG, MPI_COMM_WORLD, &status );
	printf("Dane od procesu o randze (i=%d): %s (%d)\n", 
	       i,hostnamesend, status.MPI_SOURCE );
      }
      
    }

  }
  else{
	printf("Pojedynczy proces o hostname: %s (brak komunikatów)\n", hostname);
  }

  
  MPI_Finalize(); 
  
  return(0);

}

