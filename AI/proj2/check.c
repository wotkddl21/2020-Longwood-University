#include<stdio.h>
#include<stdlib.h>


int main(void){
	char imagefile[100];
	FILE *fp;
	FILE* fp2;
	unsigned char* temp;
	int i=0;
	int idx=0;
	temp = (unsigned char*)malloc(sizeof(unsigned char));
	printf("Input imagefile: ");
	scanf("%s",imagefile);
	fp = fopen(imagefile,"rb");
	if(fp==NULL){
		printf("file open error!\n");
		printf("mnist/train-images-idx3-ubyte \n");
		return 0;
	}
	fp2 = fopen("check.pgm","wb");
	printf("input the index of imagefile (0<= index <60000) :");
	scanf("%d",&idx);
	if(idx<0 || idx>=60000){
		printf("invalid idx\n");
		printf("0<= idx < 60000\n");
		return 0;
	}
	fseek(fp,16L,SEEK_SET);
	fprintf(fp2,"P2\n#read from train image\n28 28\n255\n");
	for(i=0;i<idx;i++){
		fseek(fp,784L,SEEK_CUR);
	}
	for(i=0;i<28*28;i++){
		fread(temp,1,1,fp);
		fprintf(fp2,"%d ",*temp);
	}
	fclose(fp);
	fclose(fp2);
	free(temp);
	return 0;
}







