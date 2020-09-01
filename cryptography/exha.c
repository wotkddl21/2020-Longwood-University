#include<stdio.h>
#include<string.h>


int main(void){
	char input[1000]={""};
	char output[1000]={""};
	printf("Input string :");
	scanf("%s",input);
	for(int i=0;i<=26;i++){
	  	for(int j=0;j<strlen(input);j++){
		  	output[j] = input[j];
		  	output[j]-=26;
			output[j]+=i;
			if(output[j]<'a'){
		  		output[j]+=26;
			}
	  	}
		printf("%dth encrypt result : %s\n",i,output);
		for(int j=0;j<strlen(input);j++){
		 	output[j] = input[j];
		  	output[j]-=i;
			if(output[j]<'a'){
			  	output[j]+=26;
			}
		}
		printf("%dth decrypt result : %s\n\n",i,output);
	}

	return 0;
}
