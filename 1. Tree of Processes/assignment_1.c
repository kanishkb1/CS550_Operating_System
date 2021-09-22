	/*Course: Operating System
	Assignment-1: Tree of Processes
	
	Name: Jigeesha Sanjeev Jain
	B-Number: B00928112
	
	Name: Kanishk Barhanpurkar
	B-Number: B00863529
	*/
		

/*Header files*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/* Accessing various types of data types*/
#include <sys/types.h>
/*Declarations for waiting*/
#include <sys/wait.h>
#include<string.h>



/*Main Function*/
int main(int argc,char* argv[])
	{
	int r1;
	int r2;
	int l;
	int n;
	int a;
	int b;
	char tree[75];
	
	/*atoi-converts string into integer */
	/*sprintf-converts integer into string */
	l=atoi(argv[1]);
	
	sprintf(tree,"%d",l-1);
	char* Process[]={argv[0],tree,argv[2],0};
	
		
	printf("(%d): Process starting\n",getpid());
	printf("(%d):Parent's id=(%d)\n",getpid(),getppid());	
	printf("(%d):Level in the tree= (%d)\n",getpid(),l);
	printf("(%d): Creating (%d) children at level (%d)\n",getpid(),atoi(argv[2]),l-1);

		
		if(l>1)
			{
			for(a=0;a<atoi(argv[2]);a++)
				{
	 				r1=fork();
	 				if(r1<0){
	 				printf("Fork process Failed");
	 				/*Fork process failed */
	 			}
	 
	 
	 	if(r1==0)
	 		{
			  b=execvp(argv[0],Process);
	  		  /* represents the arguments to the file specified here it is .out file. Those will work as input to the .out file */	 
	  		  if(b==-1)
	  			{
	  				break;
	  			}
	
			}
	
		if(r1>0)
			{
			//Rectification of parent process  
			}	
	
		if(r1==-1)
			{
			break;
	
			}
	
	while(wait(NULL)<0); //terminating all processes
	}
	}
	
	printf("%d:Terminating at level (%d) \n", getpid(),l);
	return 0;
	 	
	}
