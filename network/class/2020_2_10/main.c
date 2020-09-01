#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
int main(void){
	int cpid = fork();
	if(cpid!=0){
		//parent code
		printf("I am your parent\n");
		//kill(cpid,9); // kill the child proess. So there is no child process.
		waitpid(cpid,NULL,0);
		//getpid():  get my pid.
		//getppid(): get my parent's pid.
	}
	else{
		//child code
		for(int i=0;i<210;i++){
			printf("%d\n",i);
		}
			printf("I am not the parent\n");
		// in this case, there would be commandline before "I am not the parent".
		// This is because, the parent process ends first and then the commandline appears.
		// And the child process keeps going on. Program oes not end!
		// How to solve it?
		// Use wait command
		// Add #include<sys/wait.h>
		// In parent's code, add waitpid(cpid,NULL,0);
	}
	return 0;
}
