#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include"../pomiar_czasu/pomiar_czasu.h"

int zmienna_globalna=0;

int main(){
  
  int pid, wynik, i;
  
  inicjuj_czas();
  for(i=0;i<1000;i++){
    
    pid = fork(); 			
    
    if(pid==0){ 			
      
      zmienna_globalna++;
      void func_pointer (int *zmienna)
      {
        (*zmienna) = 5;
      }

      wynik=execv("./program", NULL);
      if(wynik==-1)
           printf("Proces potomny nie wykonal programu\n");

      exit(0);
      
    } else {					
      
      wait(NULL);
      
    }
  }
  
  drukuj_czas();

}

  
