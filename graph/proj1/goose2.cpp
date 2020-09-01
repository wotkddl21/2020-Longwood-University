#include<iostream>
#include<string>
#include<time.h> //for random
#include<cstdlib>


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
				}
				if (A.position>63){
					temp = A.position-63;
					A.position = 63-temp;
				}
			}
			switch(A.position){// rule for special location
				case 6:
					A.position = 12;
					break;
				case 19:
					A.stay = 1;
					break;
				case 31:
					A.stay = 2;
					break;
				case 42:
					A.position = 39;
					break;
				case 52:
					A.stay = 3;
					break;
				case 58:
					A.position = 1;
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
	int* his_percent;
	int hisflag=0;
	int hisflag2=0;

	printf("Input num : ");
	scanf("%d",&t);
	history = (int**)malloc(sizeof(int*)*100000000);
	for(j=0;j<10000000;j++){
		history[j] = (int*)malloc(sizeof(int)*t);
	}
	current = (int*)malloc(sizeof(int)*t);
	his_percent = (int*)malloc(sizeof(int)*10000000);
	for(j=0;j<t;j++){
		his_percent[j]=0;
	}
	winnum1 =0;
	totalnum=0;
	for(i=0;i<1000000;i++){
		flag=0;
		A.position=0;
		B.position=0;
		A.stay=0;
		B.stay=0;
		first_A=true;
		first_B=true;
		totalnum++;
		for(j=0;j<t;j++){
			dice1 = rand()%6+1;
			dice2 = rand()%6+1;
			turn =1;
			run_goose(1,dice1,dice2);
			current[j] = A.position;
			if(A.position==63&&j!=t-1){
				flag=1;
				for(k=0;k<t;k++){
					current[j]=0;
				}
				break;   //win before the num.
			}

		}
		if(A.position==63 &&flag==0){ // win at "t"th steps.
			//check the history
			winnum1++;
			if(hisnum==0){
				for(k=0;k<t;k++){
					history[0][k] = current[k];
				}
				his_percent[0]++;
				hisnum++;
			}
			else{
				for(k=0;k<hisnum;k++){
					for(int l=0;l<t;l++){
						if(history[k][l]!=current[l]){
							hisflag=1;
						}
					}
					if(hisflag){ // if history[k] != current
						hisflag=0;
					}
					else{
						his_percent[k]++;
						hisflag2=1;
						break;
					}
				}
				if(hisflag2==0){
					hisflag=0;
					for(k=0;k<t;k++){
						history[hisnum][k] = current[k];
					}
					his_percent[hisnum]++;
					hisnum++;
				}
				else{
					hisflag2=0;
				}
			}
			
		}
	}
	probability = (winnum1/totalnum)*100;
	printf(" probability : %lf\n",probability);
	int max=0;
	int max_index=0;
	for(j=0;j<hisnum;j++){
		if(max<his_percent[j]){
			max = his_percent[j];
			max_index = j;
		}
	}
	for(k=0;k<t;k++){
		printf("%d ",history[max_index][k]);
	}
	printf(" percentage : %lf \%\n",(his_percent[max_index]/winnum1)*100);
	printf(" average percentage : %lf \%\n",(1/winnum1)*100);
	
	printf("winnum1 : %lf\n",winnum1);
	printf("total number of path : %d\n",hisnum);
		
return 0;

}
