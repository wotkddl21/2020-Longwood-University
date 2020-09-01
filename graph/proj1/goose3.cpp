#include<iostream>
#include<string>
#include<time.h> //for random
#include<cstdlib>

int pathnum;

int goose[1000]={0};

typedef struct _player{
	int position;
	int stay;

}Player;
Player A,B;

int turn=1;
int count=0;
bool first_A=true;
bool first_B=true;

int** history;

void run_goose(int turn,int dice1,int dice2){
	int total=0;
	int temp=0;
	total = dice1+dice2;
	if(turn){
		if(A.stay==0){
			if(first_A){
				if((dice1==6&&dice2==3)||(dice1==3&&dice2==6)){// rule 1
					A.position = 26;
				}
				else if(((dice1==4)&&(dice2==5))||((dice1==5)&&(dice2==4))){// rule 2
					A.position = 53;
				}
				else{
					A.position+=total;
				}
				first_A=false;
			}
			else{
				A.position+=total;
				if(goose[A.position]==1){ // rule 3
					A.position+=total;
					pathnum++;
				}
				if (A.position>63){
					temp = A.position-63;
					A.position = 63-temp;
				}
			}
			switch(A.position){// rule for special location
				case 6:
					A.position = 12;
				pathnum++;
					break;
				case 19:
					A.stay = 1;
				pathnum++;
					break;
				case 31:
					A.stay = 2;
				pathnum++;
					break;
				case 42:
					A.position = 39;
				pathnum++;
					break;
				case 52:
					A.stay = 3;
				pathnum++;
					break;
				case 58:
					A.position = 1;
				pathnum++;
					break;
				default:
					break;
			}
		}
		else{
			A.stay--;
		}
	}
	else{
		if(B.stay==0){
			if(first_B){
				if((dice1==6&&dice2==3)||(dice1==3&&dice2==6)){// rule 1
					B.position = 26;
				}
				else if((dice1==4&&dice2==5)||(dice1==5&&dice2==4)){// rule 2
					B.position = 53;
				}
				else{
					B.position+=total;
				}
				first_B = false;
			}
			else{
				if(goose[B.position]==1){ // rule 3
					B.position+=total+total;
				}
				else{
					B.position += total;
				}
			}
	
			if(A.position>63){  //rule 4
				temp = B.position-63;
				B.position = B.position-temp;
			}
			switch(B.position){// rule for special location
				case 6:
					B.position = 12;
					break;
				case 19:
					B.stay = 1;
					break;
				case 31:
					B.stay = 2;
					break;
				case 42:
					B.position = 39;
					break;
				case 52:
					B.stay = 3;
					break;
				case 59:
					B.position = 1;
					break;
				default:
					break;
			}
		}
		else{
			B.stay--;
		}
	}
}

int main(void){
	int num=0;
	int whosewin=0;
	char input=0;
goose[5]=goose[9]=goose[14]=goose[18]=goose[23]=goose[27]=goose[32]=goose[36]=goose[41]=goose[45]=goose[50]=goose[54]=goose[59]=1;
	srand(time(NULL));
	double totalnum=0;
	double winnum1=0;
	double probability=0.0;
	double i=0;
	int dice1=0;
	int dice2=0;
	int hisnum=0;
	int  j=0;
	int flag=0;
	int t=0;
	int k=0;
	int* current;
	int* max_path;
	int* his_percent;
	int hisflag=0;
	int hisflag2=0;
	int max_pathnum=0;
//	printf("Input num : ");
//	scanf("%d",&t);
	history = (int**)malloc(sizeof(int*)*100000000);
	for(j=0;j<10000000;j++){
		history[j] = (int*)malloc(sizeof(int)*t);
	}
	current = (int*)malloc(sizeof(int)*64);
	max_path = (int*)malloc(sizeof(int)*64);
	his_percent = (int*)malloc(sizeof(int)*10000000);
	for(j=0;j<t;j++){
		his_percent[j]=0;
	}
	winnum1 =0;
	totalnum=0;
	for(i=0;i<100000000;i++){
		flag=0;
		A.position=0;
		B.position=0;
		A.stay=0;
		B.stay=0;
		first_A=true;
		first_B=true;
		totalnum++;
		pathnum=0;
		for(k=0;k<64;k++){
			current[k]=0;
		}
		//initializing current path
		for(j=0;j<64 ;j++){ // till the game ends or some invalid thing happens
			pathnum++;
			dice1 = rand()%4+1;
			dice2 = rand()%4+1;
			turn =1;
			run_goose(1,dice1,dice2);
			current[j] = A.position;
			int temp = A.position;
			if(j>0){
				for(int l=0;l<j;l++){
					if(temp==current[l]){
						flag=2;
						break;
					}
				}
			}
			if(A.position==63){
				break;
			}
		}
		if(A.position==63&&flag==0){ // find some new path
			if(pathnum>max_pathnum){
				for(j=0;j<pathnum;j++){
					max_path[j] = current[j];
				}
				max_pathnum = pathnum;
			}

		}
	}
	for(j=0;j<max_pathnum;j++){
		printf("%d ",max_path[j]);
	}
	printf("\n max_pathnum : %d\n",max_pathnum);
return 0;

}
