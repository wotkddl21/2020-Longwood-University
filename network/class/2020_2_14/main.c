#include<stdio.h>
#include<signal.h>
#include<unistd.h>
void child(){
	union sigval value;
	value.sival_int =42;
	/*
	 union sigval{
			int sival_int;
			void* sigval_ptr;
	 }
	  */
	sigqueue(getppid(),SIGUSR1,value);  // sending value (42) to parent..


}

void handler(int sig,siginfo_t* info,void* p){
	printf("%d\n",info->si_value);

}

void parent(){
	struct sigaction action;
	action.sa_sigaction = handler;
	sigaction(SIGUSR1,&action,NULL);
	sleep(5);
}




int main(){
	int cpid=fork();
	if(cpid!=0){
		parent();
	}
	else{
		child();
	}

	return 0;
}




