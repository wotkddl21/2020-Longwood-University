#include<stdio.h>
#include<unistd.h>
#include<signal.h>

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
int main(void){

	signal(SIGINT,handler); // if input ctrl+c then the "Hello there" will appear
	while(!stopped);  
	return 0;
}
