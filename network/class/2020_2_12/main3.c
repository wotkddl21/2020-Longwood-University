#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
int stopped=0;
void handler2(int sig){
	printf("Hello again\n");
	signal(SIGINT,SIG_DFL); // change signal function to be default
}

void handler(int sig){
	printf("Hello there\n");
	signal(SIGINT,handler2);
	// it calls handler2
}

void fromChild(int sig){
	printf("child said hello\n");
	stopped=1;
}
void toParent(int sig){
	printf("hello parent\n");
	stopped=1;
}
int main(void){
	int cpid =fork();
	if(cpid!=0){
		//parent
		signal(SIGUSR1,fromChild);
		signal(SIGINT,SIG_IGN);
	}
	else{
		//child
		signal(SIGINT,toParent);
	}
	while(!stopped);  
	
	if(cpid==0){
		kill(getppid(),SIGUSR1); //send signal to SIGUSR1??
	}
	printf("%d -- exited\n",getpid());

	return 0;
}
