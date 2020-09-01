#include<stdio.h>
#include<unistd.h>
#include <sys/wait.h>
extern char** environ;
int main(){
	char* args[]={"child","12","15",NULL};
	int cpid = fork();
	if(cpid!=0){
		//parent
		wait(NULL);
	}
	else{
		execv("child",args);
	}
	int i=0;
	while(environ[i]!=NULL){
		printf("%s\n",environ[i]);
		i+=1;
	}
	printf("parent is done\n");
	return 0;
}
