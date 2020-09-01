#include<stdio.h>

int gcd(int a,int b);

int main(void){
	int m=0;
	int num=0;
	m=30;
	for(int i=2;i<m;i++){
		if(gcd(i,m)){
		  printf("%d  %d\n",m,i);
			  num++;
		}
	}
	printf("\n");
	printf("m = 30...... result : %d\n",num);
	num=0;
	m=100;
	for(int i=2;i<m;i++){
		if(gcd(i,m)){
		  printf("%d %d\n",m,i);
			  num++;
		}
	}
	printf("\n");
	printf("m = 100...... result : %d\n",num);
	num=0;
	m=1225;
	for(int i=2;i<m;i++){
		if(gcd(i,m)){
		  printf("%d %d\n",m,i);
			  num++;
		}
	}
	printf("\n");
	printf("m = 1225...... result : %d\n",num);
	num = gcd(3,26);
	num = gcd(2,26);
	return 0;
	
}

int gcd(int a,int b){
	int flag=1;
	int num=0;
	int temp=b;
	for(int i=0;i<temp;i++){
		a%=b;
		if(a==1){
  			break;
		}
		else if(a==0){
		  flag=0;
		  break;
		}
		b%=a;
		if(b==1){
		  break;
		}
		else if(b==0){
		  flag=0;
		  break;
		}
		num++;		
	}
	return flag;
}

