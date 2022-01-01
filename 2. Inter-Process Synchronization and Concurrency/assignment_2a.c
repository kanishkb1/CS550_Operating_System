
/*	
			***Assignment Details***
	Assignment-2: Inter-Process Synchronization and Concurrency
	Part-A: Multi-Process Addition Game Without Locking
	Instructor: Prof. Kartik Gopalan

			***Team Members***
	Name: Kanishk Barhanpurkar
	E-mail: kbarhan1@binghamton.edu
	B-Number: B00863529
	
	Name: Jigeesha Sanjeev Jain
	E-mail:jjain1@binghamton.edu
	B-Number: B00928112
*/


//Header Files                 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<unistd.h>
#define SHM 1024 


/*Main Function*/
	int main(int argc, char *argv[])
	{
	/*Variables for P-processes and M*/
	int P;
	int M;
	pid_t p;
	//two integers- n1 and n2.
	int wpid, status;
	int i=0;
	int *var;
	int shmid;
	key_t key;
	int sum;
	
	//variables passed through terminals 
	P=atoi(argv[1]);
	M=atoi(argv[2]);
	
	/*Creating key for the file test.shm*/
	if ((key=ftok("test.shm",'x'))<0)
	{
	perror("ftok");
	exit(1);
	}
	
	/* Creating shared memory*/
	//SHMGET-returns shared memory identifier with key
	//IPC-CREAT- creats a new segment
	//IPC_EXCL- ensures that this call creates the segment. If exists, the call fails.
	if ((shmid= shmget(key,SHM,0644 | IPC_CREAT | IPC_EXCL))<0)
	{
		
		perror("shmget");
		exit(1);
	}
	printf("Key of the shared memory is %d\n",shmid);
	
	//shmat -used function used for sharing shared memory specified by shmid and return the address by memory segment
	//var variable is used for allocating shared memory and initialized as an integer array.
	var = shmat(shmid, (void *)0, 0);
	if (var ==(int*)(-1))
	{
	perror("shmat");
	exit(1);
	}
	
	//assignment of 3 variables to the shared memory for 2 integers and winner pid.
	
	var[0]=1;
	var[1]=2;

	
	//Fork P child processes
	for (i=0;i<P;i++)
		{
			p=fork();
			if (p < 0) { 
		perror("fork failed:"); 
		exit(1); 
		}
			if (p==0)
			{
			do{
				sum=var[0] + var[1];
				printf("Pid:%d, Two Numbers: %d %d,sum: %d\n",getpid(),var[0],var[1],sum);
				if (sum > M)
				{
					var[2] = getpid();
					var[3] = sum;
					exit(2);
				}
				if (var[0] > var[1])
				{
					var[1] = sum;
				}
				else
				{
					var[0] =sum;				
				}
				}while(sum<M);
			}	
		
	
		}
		
	if (p>0)
	{
	while ((wpid = wait(&status)) > 0); 
	printf("Winner PID:%d,sum=%d",var[2],var[3]);

	//shmdt() detaches the memory segment located at the address specified by shmaddr
	if (shmdt(var) ==-1)
	{
	perror("shmdt");
	exit(1);
	}

	//function allows caller to control the shared memory segment specified by shmid parameter.
	if (shmctl(shmid, IPC_RMID, NULL) ==-1){
	perror("shmctl");
	exit(1);	
	}
	}
		
	return 0;
	}
