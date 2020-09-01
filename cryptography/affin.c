#include<stdio.h>
#include<string.h>



int main(void){
	char input[1000]={""};
	int temp[1000]={0};
	int num=0;
	int b=0;
	printf("Input string: ");
	scanf("%s",input);
	printf("Input shift a:");
	scanf("%d",&num);
	printf("Input shift b:");
	scanf("%d",&b);
	for(int i=0;i<strlen(input);i++){
	  temp[i] = input[i]-'a';
	  temp[i] = (num*(temp[i]-b))%26;
	  input[i] = temp[i]+'a';
	}
	printf("Output : %s\n",input);
	return 0;
}
