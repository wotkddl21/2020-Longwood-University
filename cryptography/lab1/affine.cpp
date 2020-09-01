#include<iostream>
#include<string>
using namespace std;

int inverse[26]={0,1,0,9,0,21,0,15,0,3,0,19,0,0,0,7,0,23,0,11,0,5,0,17,0,25};

bool a_inverse[26]={0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1};

void decrypt(string);

bool CheckE(string);
bool CheckTAOINSHR(string);
char affine(int,int,char);
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
	string temp1;
	string guess;
	//input : ciphertext
	for(a=1;a<=25;a++){
		if(a_inverse[a]!=0){
			for(b=0;b<26;b++){
				guess.clear();
				for(i=0;i<input.length();i++){
					guess.insert(i,1, affine(a,b,input[i]));
				}
				if(CheckE(guess)&&CheckTAOINSHR(guess)){
					cout<<guess<<endl;
				}

			}
		}
	}

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

bool CheckE (string input){
	//input : plaintext
	int count_e=0;
	int length = input.length();

	for(int i=0;i<length;i++){
		if(input.at(i)=='E'){
			count_e++;
		}
	}
	if(count_e*100>=8*length){
		return true;
	}
	return false;
}
bool CheckTAOINSHR(string input){
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
			return false;
		}
	}
	return true;
}


