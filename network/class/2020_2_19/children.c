#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

extern char** environ;

int main(){
	setenv("CMSC242","is great",1);
//	setenv("CMSC242","is great",1);
//	setenv("CMSC242","is great",1);
//	setenv("CMSC242","is great",1);
	int i=0;
	while(environ[i]!=NULL){
		printf("%s\n",environ[i]);
		i+=1;
	}
	return 0;
}
