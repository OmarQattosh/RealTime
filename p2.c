#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

void p2_picks_integers(int);

 
#define CHILD2_FILE "./child2.txt"

int main ()
{
printf("in p2\n\n");
      //sigset SIGUSR1 for P1
   if (sigset(SIGUSR1, p2_picks_integers) == -1) 
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



void p2_picks_integers(int the_sig)
{
 FILE *fptr_p2;
 int n2;
 
if ((fptr_p2 = fopen(CHILD2_FILE,"a")) == NULL ){
  	perror("CHILD2_FILE");
  	exit(-2);
  }

 sleep(1);
 srand(time(0));

 for(int i = 0; i < 10;i++)
 {
  n2 = (int) (1 + rand() % 99); 
  fprintf(fptr_p2,"%d\n",n2);
 }
   
   fclose(fptr_p2);
}

