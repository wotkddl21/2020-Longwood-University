#include<stdio.h>
#include<unistd.h>
#include <sys/wait.h>
int main(){
	char* args[]={"child","I","want","go","home",NULL};
	int cpid = fork();
	if(cpid!=0){
		//parent
		wait(NULL);
	}
	else{
		execv("child",args);
	}
	printf("parent is done\n");
	return 0;
}
