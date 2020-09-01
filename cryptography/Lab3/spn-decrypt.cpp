#include <iostream>
#include <cstdlib>
using namespace std;
unsigned int calc_subkey(unsigned int* key,int round,int pos){
	return (key[round]>>(8*(3-pos)))%256;
}

unsigned char substitute(unsigned char c){
	return (c-1)%256;
}

void permute(unsigned int* old_block, unsigned int* new_block){
	unsigned int check=0;
	unsigned int check1=0;
	int i=0;
	/* aaab bbbb bbbc cccc cccd dddd ddda aaaa old_block
	 * aaaa aaaa bbbb bbbb cccc cccc dddd dddd new_block
	 * */
	check = old_block[0]<<24;
	check = check>>29;
	check1 =old_block[3]<<27;
	check1 = check1>>24;
	new_block[0] = check+check1;

	check = old_block[1]<<24;
	check = check>>29;
	check1 = old_block[0]<<27;
	check1 = check1>>24;
	new_block[1] = check+check1;

	check = old_block[2]<<24;
	check = check>>29;
	check1 = old_block[1]<<27;
	check1 = check1>>24;
	new_block[2] = check+check1;

	check = old_block[3]<<24;
	check = check>>29;
	check1 = old_block[2]<<27;
	check1 = check1>>24;
	new_block[3] = check+check1;
}
void decrypt(int rounds,unsigned int* key,string block,int len){
	int i=0;
	int j=0;
	unsigned int u[4]={0};
	unsigned int v[4]={0};
	unsigned int w[4]={0};
	unsigned int temp=0;
	unsigned int subkey=0;
	for(i=0;i<len;i++){
		block[i] = block[i]^calc_subkey(key,rounds,i);
	}//XOR final key
	for(i=0;i<len;i++){
		block[i] = substitute(block[i]);
	}//substitute
	for(j=0;j<len;j++){
		w[j] = block[j];
		w[j] = w[j]^calc_subkey(key,rounds-1,j);
		block[j] = w[j];
	}//XOR
	//start inverse rounds
	for(i=rounds-2;i>=0;i--){
		for(j=0;j<len;j++){
			w[j] = block[j];
		}
		//permute
		permute(w,v);
		for(j=0;j<len;j++){
			//sub
			u[j] = substitute(v[j]);
			//XOR
			subkey = calc_subkey(key,i,j);
			w[j] = u[j]^subkey;
			block[j] = w[j];
		}
	}
	for(i=0;i<len;i++){
		printf("%c",block[i]);
	}
}

int main(int argc,char** argv){
	int rounds=0;
	unsigned int* key;
	string message="";
	string block="";
	if(argc<2){
		cout<<"Syntax: Encrypt <num_rounds> <key0> <key1> ...<KeyNr+1>"<<endl;
		return 1;
	}
	rounds = atoi(argv[1]);
	if(argc<rounds+2){
		cout<<"Syntax: Encrypt <num_rounds> <key0> <key1> ...<KeyNr+1>"<<endl;
	return 1;
	}
	key = new unsigned int[rounds+1];
	for(int i=0;i<rounds+1;i++){
		key[i] = atoi(argv[i+2]);
	}
	getline(cin,message);
	while(message.length()%4!=0){
		message+=" ";
	}
	for(int i=0;i*4<message.length();i++){
		block = message.substr(i*4,4);
		int len = block.length();
		decrypt(rounds,key,block,len);
	}
	cout<<endl;
	delete[] key;
}




