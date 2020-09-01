#include<stdio.h>
#include<string.h>



int main(void){
	char input[1000]={""};
	char output[1000]={""};
	int num=0;
	char temp;
	printf("Input string: ");
	scanf("%s",input);
	printf("Input string : %d\n",strlen(input));
	for(int i=0;i<strlen(input);i++){
		printf("%d ",input[i]-'a');
		switch(i%8){
			case 0: num = i+1;
					break;
			case 1: num = i+2;
					break;
			case 2: num = i+3;
					break;
			case 3: num = i-3;
					break;
			case 4: num = i+3;
					break;
			case 5: num = i-3;
					break;
			case 6: num = i-2;
					break;
			case 7: num = i-1;
					break;	
		}
		output[num] = input[i];
	}

	for(int i=0;i<strlen(output);i++){
		if(i%8==0){
			printf("\n");
		}
		printf("%c",output[i]);

	}
	return 0;
}
