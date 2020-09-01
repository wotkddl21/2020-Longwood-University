#include<stdio.h>
#include<unistd.h>
#include<signal.h>

int stopped=0;
void handler2(int sig){
	printf("Hello there\n");
	stopped=1;// if there is ctrl+c input, then the "stopped" will be changed.
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
