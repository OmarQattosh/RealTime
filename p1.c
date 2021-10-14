#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>


#define CHILD1_FILE "./child1.txt" 

void p1_picks_integers(int);

int main ()
{


      //sigset SIGUSR1 for P1
   if (sigset(SIGUSR1, p1_picks_integers) == -1) 
   {
      perror("Sigset can not set SIGUSR1");
      exit(SIGUSR1); 
   	
   }
   

   while(1)
   {
     pause(); 
     kill(getppid(),SIGINT);
   }

return 0;
}



void p1_picks_integers(int the_sig)
{
 FILE *fptr_p1;
 int n1;
 
if ((fptr_p1 = fopen(CHILD1_FILE,"a")) == NULL ){
  	perror("CHILD1_FILE");
  	exit(-3);
  }
  
 sleep(1); // sleep to get different seed
 srand(time(0));

 for(int i = 0; i < 10;i++)
 {
  n1 = (int) (1 + rand() % 99); 
  fprintf(fptr_p1,"%d\n",n1);
 }
   
   fclose(fptr_p1);
}

