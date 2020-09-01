#include<aspell.h>
#include<iostream>
#include<cstdlib>
#include<vector>
#include<algorithm>
#include<time.h>
using namespace std;

int affine[26] = {0,1,0,9,0,21,0,15,0,3,0,19,0,0,0,7,0,23,0,11,0,5,0,17,0,25};


int det(int**,int);
int cofactor(int**,int,int,int);
int** generator(int);
void check_matches(AspellSpeller* spell_checker,string guess){
	vector<string> matches;
	string match;
	char newChar;
	for(int i=1;i<26;i++){
		match="";
		for(int j=0;j<guess.length();j++){
			newChar = guess.at(j)-'a';
			newChar+=i;
			newChar %=26;
			newChar +='A';
			match+=newChar;
		}
		if(aspell_speller_check(spell_checker,match.c_str(),match.length())){
			matches.push_back(match);
		}
	}
	sort(matches.begin(),matches.end());
	matches.erase(unique(matches.begin(),matches.end()),matches.end());

	if(matches.size()>0){
		cout<<"Matches for "<<guess<<":";
		for(int i=0;i<matches.size();i++){
			cout<<" "<<matches[i];
		}
		cout<<endl;
	}
	
}

void check_affine(AspellSpeller* spell_checker,string guess){
	vector<string> matches;
	string match;
	string match2;
	char add;
	int i=0;
	int j=0;
	for(int a=1;a<26;a++){
		for(int b=0;b<26;b++){
			for(int a1=1;a1<26;a1++){
				for(int b1=0;b1<26;b1++){
					match="";
					if(affine[a]&& affine[a1]){
						for(j=0;j<guess.length();j++){
							add = guess.at(j)-'a';
							add*=a;
							add+=b+b1;
							add*=a1;
							add%=26;
							add+='a';
							match+=add;
						}
					}
					if(match!=guess){
						match2="";
						for(int l=0;l<match.length();l++){
							add=match.at(l)-'a'+'A';
							match2+=add;
						}
						if(aspell_speller_check(spell_checker,match2.c_str(),match2.length())){
							matches.push_back(match2);
						}
					}
				}

			}
		}
	}
	sort(matches.begin(),matches.end());
	matches.erase(unique(matches.begin(),matches.end()),matches.end());

	if(matches.size()>1){
		cout<<"Matches for "<<guess<<" with affine cipher ";
		for(i=0;i<matches.size();i++){
			cout<<" "<<matches[i];
		}
		cout<<endl;
	}
}

//to do hill cipher....
//generator for invertible matrix with 5X5
//bunch of matrix.... 
void check_hill(AspellSpeller* spell_checker,string guess){
	vector<string> matches;
	string match;
	string match2;
	char add;
	int** mat;
	int** mat2;
	int i=0;
	int j=0;
	int temp=0;
	char sum=0;
	int t=0;
	for(t=0;t<500;t++){
		mat = generator(guess.length());
		mat2 = generator(guess.length());
		match="";
		for(i=0;i<guess.length();i++){
			sum=0;
			for(j=0;j<guess.length();j++){
				add =guess.at(j)-'a';
				sum+= add*mat[i][j];
			}
			sum%=26;
			if(sum<0){
				sum*=-1;
			}
			add = sum;
			add+='A';
			match+=add;
		}
		match2="";
		for(i=0;i<match.length();i++){
			sum=0;
			for(j=0;j<match.length();j++){
				add =match.at(j)-'a';
				sum+= add*mat[i][j];
			}
			sum%=26;
			if(sum<0){
				sum*=-1;
			}
			add = sum;
			add+='A';
			match2+=add;
		}
		if(aspell_speller_check(spell_checker,match2.c_str(),match2.length())){
				matches.push_back(match2);
		}
		sort(matches.begin(),matches.end());
		matches.erase(unique(matches.begin(),matches.end()),matches.end());
	}
	if(matches.size()>0){
		cout<<"Matches for "<<guess<<" with hill cipher ";
		for(i=0;i<matches.size();i++){
			cout<<" "<<matches[i];
		}
		cout<<endl;
	}
}

int**  generator(int size){
	int** Mat;
	Mat = (int**)malloc(sizeof(int*)*size);
	int i=0;
	int j=0;
	int k=0;
	int l=0;
	int temp=0;
	for(int i=0;i<size;i++){
		Mat[i] = (int*)malloc(sizeof(int)*size);
	}
	for(i=0;i<size;i++){
		for(j=0;j<size;j++){
			Mat[i][j]=0;
		}
	}
	while(1){
		for(i=0;i<size;i++){
			for(j=0;j<size;j++){
				Mat[i][j] = rand()%26;
			}
		}
		temp = det(Mat,size)%26;
		if(temp<0){
			temp *=-1;
		}
		if(affine[temp]){
			return Mat;
			break;
		}
	}
}

int det(int** input,int size){
	int i=0;
	int ret=0;
	if(size==2){
		ret = input[0][0]*input[1][1]-input[0][1]*input[1][0];
	}
	else if(size==1){
		ret = input[0][0];
	}
	else{
		for(i=0;i<size;i++){
			ret += input[0][i]*cofactor(input,0,i,size-1);
		}
	}
	return ret;
}

int cofactor(int** input,int x,int y,int size){
	int** cof;
	int ret=0;
	cof =(int**) malloc(sizeof(int*)*(size));
	int i=0;
	int j=0;
	int a=0;
	int b=0;
	for(i=0;i<size;i++){
		cof[i] = (int*)malloc(sizeof(int)*size);
	}
	for(i=0;i<size+1;i++){
		for(j=0;j<size+1;j++){
			if(j!=y&& i!=x){
				cof[a][b] = input[i][j];
				b++;
			}
		}
		if(b==size){
			b=0;
			a++;
		}
	}
	if(x+y%2==0){ // +
		ret = det(cof,size); 
	}
	else{// -
		ret = -det(cof,size);
	}
	return ret;
}



void do_search(AspellSpeller* spell_checker,int length,char input){
	string guess;
	string end;
	int is_a_word;
	guess = "";
	end = "";

	for(int i=0;i<length;i++){
		guess+='a';
		end+='z';
	}
	while(guess!=end){
		is_a_word = aspell_speller_check(spell_checker,guess.c_str(),length);
		if(is_a_word){
//			cout<<guess<<" is a real word!"<<endl;
			if(input=='A'||input == 'a'){
				check_affine(spell_checker,guess);
			}
			else if(input=='s'||input=='S'){
				check_matches(spell_checker,guess);
			}
			else {
				check_hill(spell_checker,guess);
			}
		}
		for(int i=guess.length()-1;i>=0;i--){
			if(guess.at(i)!='z'){
				++guess[i];
				break;
			}
			else{
				guess[i]='a';
			}
		}

	}

}


int main(int argc, char* argvp[]){	
	char input=0;
	int length=0;
	AspellConfig *spell_config;
	AspellCanHaveError *possible_error;
	AspellSpeller *spell_checker;

	spell_config = new_aspell_config();
	aspell_config_replace(spell_config,"lang","en_US");
	possible_error = new_aspell_speller(spell_config);
	if(aspell_error_number(possible_error)!=0){
		cerr<<aspell_error_message(possible_error)<<endl;
		exit(EXIT_FAILURE);
	}
	spell_checker = to_aspell_speller(possible_error);
	while(1){
		cout<<"Insert S/s for shift cipher, A/a for affine cipher H/h for hill cipher: ";
		cin>>input;
		if(input == 'A'||input == 'a'||input == 's' || input == 'S'||input=='h'||input=='H'){
			while(1){
				cout<<"Input the length of word ";
				cin>>length;
				if(length<0){
					cout<<"try again  length must be over 0"<<endl;
				}
				else{
					break;
				}
			}
			do_search(spell_checker,length,input);
			break;
		}
		else {
			cout<<"Invalid input.... try again"<<endl;
		}
	}
	delete_aspell_speller(spell_checker);
	delete_aspell_config(spell_config);




	return 0;
}
