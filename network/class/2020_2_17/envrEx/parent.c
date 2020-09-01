#include <stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
extern char** environ;

int main(){
	char** var = environ;
	/*while(*var != NULL){
		printf("%s\n",*var);
		var++;
	}

	int i=0;
	while(environ[i]!=NULL){
		printf("%s\n",environ[i]);
		i++;
	}
	*/
	char* args[] = {"child",NULL};
	int cpid=fork();
	if(cpid!=0){
		wait(NULL);
	}
	else{
		execv("child",args);
	}
	char* val = getenv("CMSC242");
	printf("%s -- child says\n",val);
	return 0;
}
