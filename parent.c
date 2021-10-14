#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void p_intr(int);
void p_quit(int);
void Ref(int);



int p1_p2_fnsh_writ=0;
 int  f_des[2];
   static char message[BUFSIZ];



int main()
{
    int i, status;
    int bigScore1=0,bigScore2=0;
   
    pid_t pid,pid_arr[2];
    int pid_R;
   
  
    
    // Parent creates the pipe to talk to the Referee (R)
    if (pipe(f_des) == -1) {
        perror("Pipe");
        exit(-1);
    
    }
   printf("The Game is Starting\n "); 
   printf("Forking child 1 (P1), child 2 (P2) and the referee (R)\n ");
   
   
   
    for (i=0 ; i<3 ; i++)
    {
        pid = fork();
       

        if ( pid == 0 && i==0) {

        printf("PID1\n");
        execl("./p1","p1",NULL);
        
     
       }
          

        if ( pid == 0 && i==1){
         
         printf("PID2\n");
         execl("./p2","p2",NULL);
         
        }
            
            
 	if ( pid == 0 && i==2){
 	
 	
 	 printf("PID_R\n");
 	 printf("R\n");
   //sigset SIGUSR1 for P1
   if (sigset(SIGUSR1, Ref) == -1) 
   {
      perror("Sigset can not set SIGUSR1");
      exit(SIGUSR1); 
   	
   }
   
 	 
 	 while(1){
 	 
 	 
 	 pause();
 	 
 
        }

}
           

        else
        { 
         
            printf("Parent = %d\n",getppid());
            pid_arr[i]=pid;    
        }
    }
      printf("P1 id = %d\n",pid_arr[0]);
      printf("P2 id = %d\n",pid_arr[1]);  
  

    
    //sigset 
   if (sigset(SIGINT, p_intr) == -1) 
   {
      perror("Sigset can not set SIGINT");
      exit(SIGINT); 
   	
   }
   //sigset 
   if (sigset(SIGQUIT, p_quit) == -1) 
   {
      perror("Sigset can not set SIGQUIT");
      exit(SIGQUIT); 
   	
   }

       while (bigScore1 != 50 && bigScore2 != 50 ){

        sleep(1);
        kill(pid_arr[0], SIGUSR1);
        sleep(2);
        kill(pid_arr[1], SIGUSR1);
        
        pause();
        
        // p1 and p2 has informed the parent that they are ready
        if(p1_p2_fnsh_writ==2){
          printf("p1_p2_fnsh_writ = %d\n",p1_p2_fnsh_writ);
          p1_p2_fnsh_writ=0;
         // close(f_des[0]);
            if( write(f_des[1],"child1.txt-child2.txt",strlen("CHILD1_FILE-CHILD2_FILE")) != -1)
	     {
                printf("sent to R\n");
    	        fflush(stdout);

             }
           
             
             kill(pid_arr[2],SIGUSR1);
             
           printf("in between\n\n");
          sleep(1);
          printf("after sleep\n\n");
          if ( read(f_des[0], message, BUFSIZ) != -1 ) {
          printf("Parent: Score = %s\n", message);
          fflush(stdout);
         }
         else {
         perror("Read");
         exit(-13);
         }
       //  printf("%s\n",message);
             
 
        }
        

   }
    
    
}


void p_intr(int the_sig){

 p1_p2_fnsh_writ++;
}

void p_quit(int the_sig){

printf("p_quit\n");
exit(-11);
}

void Ref(int the_sig){
 printf("in REf\n");
 char files [2][20];
 char *tmp;
 int i=0;
 int num1,num2;
 int score1=0,score2=0;
 
 char tmp_score[8];
 char tmp_score1[3];
 
 char total_score [8];

 
 FILE *fptr_p1;
 FILE *fptr_p2;
         
         if ( read(f_des[0], message, BUFSIZ) != -1 ) {
          printf("Message received by Refeere: [%s]\n", message);
          fflush(stdout);
         }
         else {
         perror("Read");
         exit(-12);
         }
         
        char *token = strtok(message,"-");
        // sperate the files        
        while (token != NULL) {
        strcpy(files[i],token);
        tmp = strdup(files[i]);
        strcpy(files[i],"./");
        strcat(files[i],tmp);
        printf("%s ",files[i]);
        token = strtok(NULL,"-");
        i++;
        free(tmp);
        }
        
        if ((fptr_p1 = fopen(files[0],"r")) == NULL ){
  	perror("CHILD1_FILE");
  	exit(-3);
  }
  
          if ((fptr_p2 = fopen(files[1],"r")) == NULL ){
  	perror("CHILD1_FILE");
  	exit(-3);
  }
  
  for( int j = 0 ; j < 10; j++ ){
     
     fscanf(fptr_p1, "%d\n", &num1);
     fscanf(fptr_p2, "%d\n", &num2);
     printf("num1 = %d  ",num1);
     printf("num2 = %d  \n",num2);
     
     if(num1 > num2){
     score1++;    
}
    else if (num1 < num2){
     score2++;
}

}

  fclose(fptr_p1);
  fclose(fptr_p2);
  //files are deleted

  unlink(files[0]);
  unlink(files[1]);
  
  sprintf(tmp_score,"%d",score1);
  strcat(tmp_score,"-");
  sprintf(tmp_score1,"%d",score2);
  strcat(tmp_score,tmp_score1);
  
 
  //close(f_des[0]);
          if( write(f_des[1],tmp_score,strlen(tmp_score)) != -1)
	     {
                printf(" R sent to parent\n");
    	        fflush(stdout);

             }

  
  printf("%s\n",tmp_score);
    
  

}


