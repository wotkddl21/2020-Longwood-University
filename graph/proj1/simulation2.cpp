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

int* history_dice1;
int* history_dice2;

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
				if (A.position>63){
					temp = A.position-63;
					A.position = 63-temp;
				}
				if(goose[A.position]==1){ // rule 3
					A.position+=total;
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
				case 59:
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
}	

int main(void){
	int num=0;
	int whosewin=0;
	char input=0;
goose[5]=goose[9]=goose[14]=goose[18]=goose[23]=goose[27]=goose[32]=goose[36]=goose[41]=goose[50]=goose[54]=goose[59]=1;
	srand(time(NULL));
	double totalnum=0;
	double winnum1=0;
	double probability=0.0;
	double p[10000];
	double i=0;
	int pathnum=0;
	int min_path=10000;
	int dice1=0;
	int dice2=0;
	int  j=0;
	int flag=0;
	int t=0;
	double temp1=0;
	for(i=0;i<100000000;i++){
		flag=0;
		A.position=0;
		B.position=0;
		A.stay=0;
		B.stay=0;
		first_A=true;
		first_B=true;
		pathnum=0;
		for(; ;){
			pathnum++;
			dice1 = rand()%6+1;
			dice2 = rand()%6+1;
			turn =1;
			run_goose(1,dice1,dice2);
			if(A.position==63){
				break;
			}
		}
		if(A.position==63){
			if(min_path>pathnum){
				min_path = pathnum;
			}
		}
	}
	printf("fewest path : %d\n",min_path);
	return 0;

}
