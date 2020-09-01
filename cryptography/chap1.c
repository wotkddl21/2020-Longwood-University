#include<stdio.h>
#include<string.h>



int main(void){
	char key[26]={'h','o','r','s','e','z','a','b','c','d','f','g','i','j','k','l','m','n','p','q','t','u','v','w','x','y'};
	char input[1000]={""};
	int i=0;
	int j=0;
	printf("Input : ");
	scanf("%s",input);
	for(i=0;i<strlen(input);i++){
	/*	for(j=0;j<26;j++){
			if(input[i] == key[j]){
			  break;
			}
		}
		input[i] = j+'a';
		*/
		input[i] = key[input[i]-'a'];
	}
	printf("result : %s\n",input);
	return 0;
}
