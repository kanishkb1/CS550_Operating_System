//Header files
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include<sys/time.h>
#include<time.h>
#define SHM 102400

typedef struct shmseg
{
	int *var;
	int wpid;
	sem_t sem;
} shmseg;

/*Main Function*/
int main(int argc, char *argv[])
{
  struct timeval start,end;
  gettimeofday(&start,NULL); 
     
	/*Variables for P-processes and M*/
	int P;
	int M;
	pid_t p;
	int status;
	int i = 0;
	int shmid;
	key_t key;
	int sum;
	int r;
	// variables passed through terminals
	P = atoi(argv[1]);
	M = atoi(argv[2]);

	struct shmseg *shmp;
  
 
  
  
	/*Creating key for the file test.shm*/
	if ((key = ftok("test.shm", 'x')) < 0)
	{
		perror("ftok");
		exit(1);
	}
 
	/* Creating shared memory*/
	// SHMGET-returns shared memory identifier with key
	// IPC-CREAT- creats a new segment
	// IPC_EXCL- ensures that this call creates the segment. If exists, the call fails.

	if ((shmid = shmget(key, sizeof(struct shmseg), 0644 | IPC_CREAT | IPC_EXCL)) < 0)
	{

		perror("shmget\n");
		exit(1);
	}
	printf("Key of the shared memory is %d\n", shmid);

	// shmat -used function used for sharing shared memory specified by shmid and return the address by memory segment
	// var variable is used for allocating shared memory and initialized as an integer array.	
	shmp = shmat(shmid, NULL, 0);
	if (shmp == (void *)(-1))
	{
		perror("shmat\n");
		exit(1);
	}

//Initialization using sem_init
	r = sem_init(&shmp->sem, 1, 2);
	if (r != 0)
	{
		printf("could not init..\n");
		exit(1);
	}
        int a = 2, b = 1;
				shmp->var = &a;
				shmp->var = &b;
   
// Fork P child processes
for (i = 0; i <= P; i++)
    {
		  p = fork();
              
    if (p < -1)
		    {  
			    perror("fork failed:");
	        exit(1);
		    }
    if (p == 0)   
        {
           
          while (1)
			            {
                    gettimeofday(&start, NULL);   
                  while (sem_trywait(&shmp->sem)<0){
                        if(errno==EAGAIN)
                         continue;
                        else 
                        perror("sem try_wait failed");
                  }
                   gettimeofday(&end, NULL);   
                  long seconds = (end.tv_sec - start.tv_sec);
                long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
                printf("The elapsed time is %d seconds and %d micros\n", seconds, micros);         
               do {
                   sum = shmp->var[0] + shmp->var[1];
                   printf("Pid:%d, Two Numbers: %d %d,sum: %d\n", getpid(), shmp->var[0], shmp->var[1], sum);
                 
                   if ( sum > M ) {
                     	shmp->var[4] = getpid();
						printf("\n Winner PID : %d \n ", getpid());
                        shmp->var[3] = sum;
                        exit(2);
                   }
               	  
                     if (shmp->var[0] > shmp->var[1]) 
						          shmp->var[1] = sum;
					        else
						          shmp->var[0] = sum;
                } while(sum<M);        
                 
            }
  }
 }
sem_post(&shmp->sem);
 return 0;
}