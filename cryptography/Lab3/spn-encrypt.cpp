#include <iostream>
#include <cstdlib>
using namespace std;
unsigned int calc_subkey(unsigned int* key,int round,int pos){
	return (key[round]>>(8*(3-pos)))%256;
}

unsigned char substitute(unsigned char c){
	return (c+1)%256;
}

void permute(unsigned int* old_block, unsigned int* new_block){
	int i=0;
	unsigned int check=0;
	unsigned int check1=0;
	// aaaa aaaa aaaa aaaa aaaa aaaa aaaa aaaa
	/* aaaa aaaa bbbb bbbb cccc cccc dddd dddd old_block
	 * aaab bbbb bbbc cccc cccd dddd ddda aaaa new_block
	 * */
	check = old_block[0]<<29;
	check = check>>24;
	check1 = old_block[1]>>3;
	new_block[0] = check+check1;
	
	
	check = old_block[1]<<29;
	check = check>>24;
	check1 = old_block[2]>>3;
	new_block[1] = check+check1;
	
	check = old_block[2]<<29;
	check = check>>24;
	check1 = old_block[3]>>3;
	new_block[2] = check+check1;

	check = old_block[3]<<29;
	check = check>>24;
	check1 = old_block[0]>>3;
	new_block[3] = check+check1;
}
void encrypt(int rounds,unsigned int* key,string block,int len){
	int i=0;
	int j=0;
	unsigned int u[4]={0};
	unsigned int v[4]={0};
	unsigned int w[4]={0};
	unsigned int temp=0;
	unsigned int subkey=0;
	for(i=0;i<rounds-1;i++){
		for(j=0;j<len;j++){
			w[j] = block[j];
			//get subkey
			subkey = calc_subkey(key,i,j);
			//XOR
			u[j] = w[j]^subkey;
			//sub
			v[j] = substitute(u[j]);
		}
		permute(v,w);
		for(j=0;j<len;j++){
			block[j] = w[j];
		}
	}
	for(j=0;j<len;j++){
		w[j] = block[j];
		w[j] =w[j]^calc_subkey(key,rounds-1,j);
		block[j] = w[j];
	}
	for(i=0;i<len;i++){
		block[i] = substitute(block[i]);
	}
	for(i=0;i<len;i++){
		block[i] = block[i]^calc_subkey(key,rounds,i);
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
		encrypt(rounds,key,block,len);
	}
	cout<<endl;
	delete[] key;
}




