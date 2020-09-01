#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main(){
	setenv("CMSC242","is great",1); // pass "CMSC242" , "is great "  to environment variable to be overwritten because of 1.




	return 0;
}
