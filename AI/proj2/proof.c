#include<stdio.h>
#include <math.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

double sigmoid(double x);

float dsigmoid(float x);
int main(int argc,char* argv[]){
	FILE* fp;//train image label
	FILE* fp1;//train image pixel
	FILE* fp2;//t10k image label
	FILE* fp3;//t10k image pixel
	FILE* fp4;
	char trainlabel_idx1[100];
	char trainimage_idx3[100];
	char t10klabel_idx1[100];
	char t10kimage_idx3[100];
	unsigned char* temp;
	unsigned char* trainlabel;
	unsigned char** trainpixel;
	unsigned char* testlabel;
	unsigned char** testpixel;
	float outputlayer[10];
	int i=0;
	int j=0;
	int k=0;
	int layernum=0;
	int* layer;
	double** neuron;
	double*** weight;
	double** neuerror;
	int epoch=0;
	double learningrate=0.0;
	int miss[10][10]={0};
	int tmiss[10][10]={0};
	trainlabel = (unsigned char*)malloc(sizeof(unsigned char)*60000);
	trainpixel = (unsigned char**)malloc(sizeof(unsigned char*)*60000);
	for(i=0;i<60000;i++){
		trainpixel[i] = (unsigned char*)malloc(sizeof(unsigned char)*784);
	}
	printf("argc : %d\n",argc);
	testlabel = (unsigned char*)malloc(sizeof(unsigned char)*10000);
	testpixel = (unsigned char**)malloc(sizeof(unsigned char*)*10000);
	for(i=0;i<10000;i++){
		testpixel[i] = (unsigned char*)malloc(sizeof(unsigned char)*784);
	}
	if(argc==1){

		printf("./proj2 <epoch> <learningrate> <layernum> <mnist/train-labels-idx1-ubyte> <mnist/train-imagefile-idx3-ubyte> <mnist/t10k-labels-idx1-ubyte> <mnist/t10k-imagefile-idx3-ubyte>\n");
		return 0;
	}
	epoch = atoi(argv[1]);
	learningrate = atof(argv[2]);
	layernum = atoi(argv[3]);
	temp = (unsigned char*)malloc(sizeof(unsigned char));
	if(argc != 8){
		printf("Invalid input\n");
		printf("argv : %d layernum : %d\n",argc,layernum);
		printf("./proj2 <epoch> <learningrate> <layernum> <mnist/train-labels-idx1-ubyte> <mnist/train-imagefile-idx3-ubyte> <mnist/t10k-labels-idx1-ubyte> <mnist/t10k-imagefile-idx3-ubyte>\n");
		return 0;
	}
	

	while(1){
		fp = fopen(argv[4],"rb");//train label
		if(fp==NULL){
			printf("file open error! 1\n");
			printf("try again!\n");
			printf("train-labels-idx1-ubyte\n");
			printf("Insert image file: ");
			scanf("%s",argv[4]);
		}
		else{
			strcpy(trainlabel_idx1,argv[4]);
			break;
		}
	}
	while(1){
		fp1 = fopen(argv[5],"rb");//train pixel
		if(fp1==NULL){
			printf("file open error! 2\n");
			printf("try again\n");
			printf("train-images-idx3-ubyte\n");
			printf("Insert image file: ");
			scanf("%s",argv[5]);
		}
		else{
			strcpy(trainimage_idx3,argv[5]);
			break;
		}
	}
	while(1){
		fp2 = fopen(argv[6],"rb");
		if(fp2==NULL){
			printf("file open error! 3\n");
			printf("try again!\n");
			printf("t10k-labels-idx1-ubyte\n");
			printf("Insert label file: ");
			scanf("%s",argv[6]);
		}
		else{
			strcpy(t10klabel_idx1,argv[6]);
			break;
		}

	}
	while(1){
		fp3 = fopen(argv[7],"rb");
		if(fp3==NULL){
			printf("file open error! 4\n");
			printf("try again!\n");
			printf("t10k-images-idx3-ubyte\n");
			printf("Insert label file: ");
			scanf("%s",argv[7]);
		}
		else{
			strcpy(t10kimage_idx3,argv[7]);
			break;
		}

	}
	//initializing neuron and weight
	neuron = (double**)malloc(sizeof(double*)*2);
	neuerror = (double**)malloc(sizeof(double*));
	weight = (double***)malloc(sizeof(double**)*2);
	neuron[0] = (double*)malloc(sizeof(double)*785);
	neuron[1] = (double*)malloc(sizeof(double)*(layernum+1));

	printf("argc2 : %d\n",argc);
	neuerror[0] = (double*)malloc(sizeof(double)*(layernum+1));

	neuron[0][784]=-1;
	neuron[1][layernum]=-1;

	weight[0] = (double**)malloc(sizeof(double*)*785);
	weight[1] = (double**)malloc(sizeof(double*)*(layernum+1));
	srand(time(NULL));
	for(i=0;i<785;i++){
		weight[0][i] = (double*)malloc(sizeof(double)*layernum);
		for(k=0;k<layernum;k++){
			weight[0][i][k] = (float)(rand()%200/100.0)-1;
		}
	}
	printf("argc3 : %d\n",argc);
	for(i=0;i<=layernum;i++){
		weight[1][i] = (double*)malloc(sizeof(double)*10);
		for(k=0;k<10;k++){
			weight[1][i][k] = (float)(rand()%200/100.0)-1;
		}
	}
	
	printf("initializing\n");
// open file
	int index=0;
	unsigned char* label;
	label = (unsigned char*)malloc(sizeof(unsigned char));
	fseek(fp,8L,SEEK_SET);
	fseek(fp1,16L,SEEK_SET);
	for(index=0;index<60000;index++){
		if(index%5000==0){
			printf("read %d/60000 images\n",index);
		}
		for(i=0;i<28*28;i++){
			fread(temp,1,1,fp1);
			trainpixel[index][i] = 255-*temp;
		}
		fread(label,1,1,fp);
		trainlabel[index]=*label;
	}
	printf("read 60000/60000 images\n");
	fseek(fp2,8L,SEEK_SET);
	fseek(fp3,16L,SEEK_SET);
	for(index =0;index<10000;index++){
		if(index%5000==0){
			printf("read %d/10000 images\n",index);
		}
		for(i=0;i<28*28;i++){
			fread(temp,1,1,fp3);
			testpixel[index][i] = 255-*temp;
		}
		fread(label,1,1,fp2);
		testlabel[index]=*label;
	}
	printf("read 10000/10000 images\n");
/// train
//
//forward
	int t=0;
	double sum=0;
	double error=0;
	double max=0;
	double error1[10]={0.0};
	printf("forward\n");
	double corr=0.0;
	int p=0;
	int maxindex=0;
	for(p=0;p<epoch;p++){
		corr=0;
		for(k=0;k<60000;k++){
			if(k==59999){
				for(i=0;i<10;i++){
					for(j=0;j<10;j++){
						printf("%5d",miss[i][j]);
						miss[i][j]=0;
					}
					printf("\n");
				}
			}
			for(i=0;i<784;i++){
			neuron[0][i] = (double)trainpixel[k][i]/255;
			}
			neuron[0][784]=-1;
			//forwarding
			for(i=0;i<layernum;i++){
				sum=0;
				for(j=0;j<785;j++){
					sum+= neuron[0][j]*weight[0][j][i];
				}
				neuron[1][i] = sigmoid(sum);
			}
			for(i=0;i<10;i++){
				sum=0;
				for(j=0;j<layernum+1;j++){
					sum+=neuron[1][j]*weight[1][j][i];		
				}
				outputlayer[i] = sigmoid(sum);
			}
			max = outputlayer[0];
			maxindex =0;
			for(i=0;i<10;i++){
				if(outputlayer[i]>=max){
					max = outputlayer[i];
					maxindex=i;
				}
			}
			if(maxindex == trainlabel[k]){
				corr+=1;
			}else{
				miss[maxindex][trainlabel[k]]++;
			}
			if(k%5000==0 && k>0){
				printf("training %d/60000 images with %d / %d \n",k,p+1,epoch);
				printf("\n");
				printf("%f % \n",(corr/(double)k)*100);
			}	
	//backpropogation
		// output
			for(i=0;i<10;i++){
				if(i==trainlabel[k]){
					error1[i] = 1-outputlayer[i];
				}
				else{
					error1[i] = 0-outputlayer[i];
				}
			}// e_k = target_k-output_k
			for(i=0;i<=layernum;i++){
					neuerror[0][i]=0;
			}
			//initializing neuron error
			//error :  target-output
		//hidden layer
			for(j=0;j<=layernum;j++){
				for(i=0;i<10;i++){
					neuerror[0][j] += outputlayer[i]*(1-outputlayer[i])*error1[i]*weight[1][j][i];
					weight[1][j][i] += learningrate*outputlayer[i]*(1-outputlayer[i])*error1[i]*neuron[1][j];
				}	
			}
			for(j=0;j<785;j++){
				for(i=0;i<=layernum;i++){
					weight[0][j][i] += learningrate*neuron[0][j]*neuerror[0][i];
				}
			}
		}
	//
	

// test
		corr=0;
		for(k=0;k<10000;k++){
			if(k==9999){
				for(i=0;i<10;i++){
					for(j=0;j<10;j++){
						printf("%5d",tmiss[i][j]);
						tmiss[i][j]=0;
					}
					printf("\n");
				}
			}

			for(i=0;i<784;i++){
				neuron[0][i] = (double)testpixel[k][i]/255;
			}
			neuron[0][784]=-1;
			//forwarding
			for(i=0;i<layernum;i++){
				sum=0;
				for(j=0;j<785;j++){
					sum+= neuron[0][j]*weight[0][j][i];
				}
				neuron[1][i] = sigmoid(sum);
			}
			for(i=0;i<10;i++){
				sum=0;
				for(j=0;j<layernum+1;j++){
					sum+=neuron[1][j]*weight[1][j][i];		
				}
				outputlayer[i] = sigmoid(sum);
			}
			max = outputlayer[0];
			maxindex =0;
			for(i=0;i<10;i++){
				if(outputlayer[i]>=max){
					max = outputlayer[i];
					maxindex=i;
				}
			}
			if(maxindex == testlabel[k]){
				corr+=1;
			}
			else{
				tmiss[maxindex][trainlabel[k]]++;
			}

			if(k%2000==0 && k>0){
				printf("testing %d/10000 images with %d / %d \n",k,p+1,epoch);
				printf("\n");
				printf("In test, accuracy %f%\n",(corr/(double)k)*100);
				printf("\n");
			}	
		}
	}
	for(i=0;i<60000;i++){
		free(trainpixel[i]);
	}
	for(i=0;i<10000;i++){
		free(testpixel[i]);
	}
	fclose(fp);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	free(trainlabel);
	free(trainpixel);
	free(testlabel);
	free(testpixel);
	free(label);
	free(temp);

	return 0;
}
double sigmoid(double x){
	return 1/(1+exp(-x));
};

float dsigmoid(float x){
	return sigmoid(x)*(1-sigmoid(x));
};

