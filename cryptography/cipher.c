#include<stdio.h>
#include<string.h>



int main(void){
	char input[1000]={""};
	int num=0;
	printf("Input string: ");
	scanf("%s",input);
	printf("Input shift num:");
	scanf("%d",&num);
	for(int i=0;i<strlen(input);i++){
	  input[i]-=num;
	  if(input[i]<'a'){
	    input[i]+=26;
	  }
	}
	printf("Output : %s\n",input);
	return 0;
}
