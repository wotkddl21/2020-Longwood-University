#include<iostream>
#include<string>
#include<vector>
using namespace std;

int inverse[26]={0,1,0,9,0,21,0,15,0,3,0,19,0,0,0,7,0,23,0,11,0,5,0,17,0,25};

bool a_inverse[26]={0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1};

void decrypt(string);

int CheckE(string);
int CheckTAOINSHR(string);
char affine(int,int,char);


int Check_double(string);
int Check_diagraph(string);
int Check_triagraph(string);
int Check_first(string);
int Check_last(string);

int main(){
	string input;
	int i=0;
	int a=0;
	int b=0;

	while( getline(cin,input)){
		decrypt(input);
		
	}


	return 0;
}
void decrypt(string input){
	int a=1;
	int b=0;
	int i=0;
	int score=0;
	string temp1;
	string guess;
	vector<int> total_score;
	vector<string> total_list;
	//input : ciphertext
	int* ind;
	int *score_ind;
	if(input.length()==0){
		return ;
	}
	for(a=1;a<=25;a++){
		if(a_inverse[a]!=0){
			for(b=0;b<26;b++){
				guess.clear();
				for(i=0;i<input.length();i++){
					guess.insert(i,1, affine(a,b,input[i]));
				}
				score+=CheckE(guess);
				score+=CheckTAOINSHR(guess);
				score +=Check_double(guess);
				score +=Check_diagraph(guess);
				score+=Check_triagraph(guess);
				score+=Check_first(guess);
				score+=Check_last(guess);
				total_list.push_back(guess);
				total_score.push_back(score);
				score=0;
			}
		}
	}
	int temp=0;
	int j=0;
	ind = (int*)malloc(sizeof(int)*total_list.size());
	score_ind = (int*)malloc(sizeof(int)*total_list.size());
	for(i=0;i<total_list.size();i++){
		ind[i] = i;
		score_ind[i] = total_score[i];
	}

	for(j=0;j<total_list.size()-1;j++){
		for(i=0;i<total_list.size()-j-1;i++){
			if(score_ind[i]<score_ind[i+1]){
				temp = score_ind[i];
				score_ind[i] = score_ind[i+1];
				score_ind[i+1]=temp;
				temp = ind[i];
				ind[i]=ind[i+1];
				ind[i+1]=temp;
			}
		}
	}
	cout<<total_list[ind[0]]<<"         "<<endl;
	for(i=1;i<total_list.size();i++){
		if(total_score[ind[i]]*100 >=95*total_score[ind[0]]){
			cout<<total_list[ind[i]]<<"         "<<endl;
		}
	}
	free(ind);
	free(score_ind);
}
char affine(int a,int b,char input){
	if(input<'A' || input>'Z'){
		return input; // do nothing
	}
	char ciphertext;
	char plaintext;
	ciphertext = input-'A';
	plaintext = ((ciphertext + 26 - b)*inverse[a])%26;
	return plaintext+'A';
}

int CheckE (string input){
	//input : plaintext
	int count_e=0;
	int length = input.length();

	for(int i=0;i<length;i++){
		if(input.at(i)=='E'){
			count_e++;
		}
	}
	if(count_e*100>=8*length){
		return 1000;
	}
	return 0;
}
int CheckTAOINSHR(string input){
	int flag=1;
	int count=0;
	int i=0;
	int j=0;
	int length = input.length();
	char list[8] = {'T','A','O','I','N','S','H','R'};
	for(j=0;j<8;j++){
		for(i=0;i<length;i++){
			if(input.at(i)==list[j]){
				count++;
			}
		}
		if(count==0){
			flag=1;
		}
		else if(count*100>=3*length&&100*count<=12*length){
			flag=1;
			count=0;
		}
		else{
			flag=0;
			return 0;
		}
	}
	return 1000;
}
int Check_first(string input){
	char list[4]={'X','Q','U','Z'};
	int i=0;
	int flag=1;
	int j=0;
	int score=0;
	for(i=0;i<4;i++){
		if(input.at(0)==list[i]){
			score-=5000;
		}
	}
	for(i=0;i<input.length()-1;i++){
		if(input.at(i)==' '){
			for(j=0;j<4;j++){
				if(input.at(i+1)==list[j]){
					score-=5000;
				}
			}
		}
	}
	return score;
};
int Check_last(string input){
	char last[4]={'X','J','Q','Z'};
	int i=0;
	int flag=0;
	int j=0;
	int score=0;
	for(i=0;i<input.length()-1;i++){
		for(j=0;j<4;j++){
			if(input.at(i+1)==' '){
				if(input.at(i)==last[j]){
					score-=1000;
				}
			}
		}
	}
	for(j=0;j<4;j++){
		if(input.at(input.length()-1)==last[j]){
			score-=1000;
		}
	}



	return score;
};
int Check_double(string input){  // return 0  if no double words, return -100 if double words exists, but invalid  return 100 if valid doubled words exist
	int flag=-1;
	int i=0;
	int j=0;
	char list[9]={'L','S','E','O','T','F','P','R','M'};
	for(i=0;i<input.length()-1;i++){
		if(input.at(i)==input.at(i+1)){
			for(j=0;j<9;j++){
				if(input.at(i)==list[j]){
					flag+=500;
				}
			}
			flag =-500;
		}
	}
	return flag;
};
int Check_diagraph(string input){
	char list[20][3] = {"TH","HE","IN","ER","AN","RE","EN","ND","ED","AR","ES","TO","NT","EA","OU","NG","ST","AT","UE","VE"};
	int i=0;
	int j=0;
	int k=0;
	int flag=0;
	int score=0;
	for(i=0;i<input.length()-1;i++){
		for(j=0;j<20;j++){
			for(k=0;k<2;k++){
				if(input.at(k+i)==list[j][k]){
					flag=1;
				}
				else{
					flag=0;
				}
			}
			if(flag==1){
				score+=1000;
			}
		}
	}
	return score;
	
};
int Check_triagraph(string input){
	int i=0;
	int j=0;
	int k=0;
	int flag=1;
	int score=0;
	char list1[7][4]={"THE","AND","HER","FOR","ARE","WAS","ALL"};
	char list2[9][4]={"ING","NTH","ENT","THA","INT","ERE","DTH","EST","IVE"};
	for(i=0;i<input.length()-4;i++){
		if(input.at(i)==' '&&input.at(i+4)==' '){  // the and her for
			for(j=0;j<7;j++){
				for(k=0;k<3;k++){
					if(input.at(i+k)==list1[j][k]){
						flag=1;
					}
					else{
						flag=0;
					}
				}
				if(flag==1){
					score+= 2000;
				}
			}
		}
	}
	flag=0;
	if(input.length()>5){
		i = input.length()-1;
		for(j=0;j<9;j++){
			for(k=0;k<3;k++){
				if(input.at(i-k)==list2[j][k]){
					flag=1;
				}
				else{
					flag=0;
				}
			}
			if(flag==1){
				score+= 1000;
			}
		}
	}
	return score;
};
