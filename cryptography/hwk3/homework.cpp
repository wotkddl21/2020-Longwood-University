#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<cstring>
using namespace std;
unsigned int Rcon[10]={
	0x01000000,0x02000000,0x04000000,0x08000000,0x10000000,0x20000000,0x40000000,0x80000000,0x1b000000,0x36000000

};
int ary[16]={
	2,3,1,1,
	1,2,3,1,
	1,1,2,3,
	3,1,1,2
};
unsigned char sbox[] = {
	0x63,0x7C,0x77,0x7B,0xF2,0x6B,0x6F,0xC5,0x30,0x01,0x67,0x2B,0xFE,0xD7,0xAB,0x76,
	0xCA,0x82,0xC9,0x7D,0xFA,0x59,0x47,0xF0,0xAD,0xD4,0xA2,0xAF,0x9C,0xA4,0x72,0xC0,
	0xB7,0xFD,0x93,0x26,0x36,0x3F,0xF7,0xCC,0x34,0xA5,0xE5,0xF1,0x71,0xD8,0x31,0x15,
	0x04,0xC7,0x23,0xC3,0x18,0x96,0x05,0x9A,0x07,0x12,0x80,0xE2,0xEB,0x27,0xB2,0x75,
	0x09,0x83,0x2C,0x1A,0x1B,0x6E,0x5A,0xA0,0x52,0x3B,0xD6,0xB3,0x29,0xE3,0x2F,0x84,
	0x53,0xD1,0x00,0xED,0x20,0xFC,0xB1,0x5B,0x6A,0xCB,0xBE,0x39,0x4A,0x4C,0x58,0xCF,
	0xD0,0xEF,0xAA,0xFB,0x43,0x4D,0x33,0x85,0x45,0xF9,0x02,0x7F,0x50,0x3C,0x9F,0xA8,
	0x51,0xA3,0x40,0x8F,0x92,0x9D,0x38,0xF5,0xBC,0xB6,0xDA,0x21,0x10,0xFF,0xF3,0xD2,
	0xCD,0x0C,0x13,0xEC,0x5F,0x97,0x44,0x17,0xC4,0xA7,0x7E,0x3D,0x64,0x5D,0x19,0x73,
	0x60,0x81,0x4F,0xDC,0x22,0x2A,0x90,0x88,0x46,0xEE,0xB8,0x14,0xDE,0x5E,0x08,0xDB,
	0xE0,0x32,0x3A,0x0A,0x49,0x06,0x24,0x5C,0xC2,0xD3,0xAC,0x62,0x91,0x95,0xE4,0x79,
	0xE7,0xC8,0x37,0x6D,0x8D,0xD5,0x4E,0xA9,0x6C,0x56,0xF4,0xEA,0x65,0x7A,0xAE,0x08,
	0xBA,0x78,0x25,0x2E,0x1C,0xA6,0xB4,0xC6,0xE8,0xDD,0x74,0x1F,0x4B,0xBD,0x8B,0x8A,
	0x70,0x3E,0xB5,0x66,0x48,0x03,0xF6,0x0E,0x61,0x35,0x57,0xB9,0x86,0xC1,0x1D,0x9E,
	0xE1,0xF8,0x98,0x11,0x69,0xD9,0x8E,0x94,0x9B,0x1E,0x87,0xE9,0xCE,0x55,0x28,0xDF,
	0x8C,0xA1,0x89,0x0D,0xBF,0xE6,0x42,0x68,0x41,0x99,0x2D,0x0F,0xB0,0x54,0xBB,0x16,
};
void transpose(unsigned int* input){
	unsigned int temp[4];
	int i=0;
	for(i=0;i<4;i++){
		temp[i]=input[i];
		input[i]=0;
	}
	for(i=0;i<4;i++){
		input[i]|=temp[0]>>(8*(3-i))&255;
		input[i]<<=8;
		input[i]|=temp[1]>>(8*(3-i))&255;
		input[i]<<=8;
		input[i]|=temp[2]>>(8*(3-i))&255;
		input[i]<<=8;
		input[i]|=temp[3]>>(8*(3-i))&255;
	}

}
unsigned char SubBytes(unsigned char c)
{
	return sbox[c];
}
unsigned int SubWord(unsigned int w)
{
	unsigned int val = 0;
	val = SubBytes(w & 255);
	w >>= 8;
	val |= SubBytes(w & 255) << 8;
	w >>= 8;
	val |= SubBytes(w & 255) << 16;
	w >>= 8;
	val |= SubBytes(w & 255) << 24;
	return val;
}
unsigned int RotWord(unsigned int val)
{
	return (val << 8) | (val >> 24);
}
unsigned char multi(unsigned char a,unsigned char b){
	/*unsigned char temp;
	if(b==0){
			return 0;
	}
	else if(b==1){
		return a;
	}
	else if(b==2){
			a = a<<1;
			return a;
	}
	else if(b==3){
			temp = a;
			temp = temp<<1;
			a ^= temp;
			return a;
	}
	return 0;*/
	unsigned char p = 0;
	unsigned char carry = 0;
	//Loop through the digits of b.
	for (int i = 0; i < 8; ++i) {
		if (b & 1) {    //if rightmost bit is one
			p ^= a; //"add" a to our sum
		}
		//Check if shifting a will produce x^8.
		carry = ((a & 128) > 0);
		a <<= 1; //shift a -- this is like multiplying by 2 or leaving a "0" at the end
				 //If there was an x^8, then since we're modding by x^8 + x^4 + x^3 + x + 1, x^8 = x^4 + x^3 + x + 1
		if (carry) {
			a ^= 0x1b; //We've already shifted off the x^8, so we just XOR with x^4+x^3+x+1 = 0001 1011 = 0x1b
		}
		b >>= 1; //shift over so we can repeat
	}
	return p;
}
void rearrange(char* input,unsigned int* key){
	int i=0;
	for(i=0;i<32;i++){
		if(input[i]>='0' && input[i]<='9'){
			input[i]-='0';
		}
		else if(input[i]>='A' && input[i]<='F'){
			input[i]-='A'-10;
		}
	}
	for(i=0;i<4;i++){
		key[i] = input[8*i]*256*256*256*16+input[8*i+1]*256*256*256+input[8*i+2]*16*256*256+input[8*i+3]*256*256+input[8*i+4]*256
			*16+input[8*i+5]*16*16 + input[8*i+6]*16 +	input[8*i+7];
	}
}
int main(int argc,char* argv[]){
	char input[32];
	char plain[32];
	unsigned int key[44];
	unsigned int state[4];
	unsigned int tempw[4];
	int i=0;
	strcpy(input,argv[1]);
	strcpy(plain,argv[2]);
	rearrange(input,key);
	rearrange(plain,state);
	transpose(state);
/*	for(i=0;i<4;i++){
		printf("%x\n",key[i]);
	}
	printf("\n");*/
	unsigned int temp;
	for(i=4;i<44;i++){
		temp = key[i-1];
		if(i%4==0){
			temp = SubWord(RotWord(temp))^Rcon[i/4-1];
		}
		key[i] = key[i-4]^temp;
	}//keyexpansion

	transpose(key);
	/*	for(i=0;i<4;i++){
			printf("%x \n",key[i]);
		}
		printf("test : %x\n",multi(0x63,2));
	*/	printf("Key0\n");
	for (i = 0; i < 44; i++) {
		printf("%x\n", key[i]);
		if (i % 4 == 3) {
			printf("\n");
		}
	}
	
	unsigned int snew[4]={0};
	for(i=0;i<4;i++){
		state[i]^=key[i];
	}
	for(int j=1;j<10;j++){
		for(i=0;i<4;i++){
			state[i]=SubWord(state[i]);
		}//s-box
		state[1] = RotWord(state[1]);
		state[2] =RotWord(RotWord(state[2]));
		state[3] = RotWord(RotWord(RotWord(state[3]))); // permute

		for(i=0;i<4;i++){
			snew[i] =0;
		}
		//mixcolumn
		for(i=0;i<4;i++){
			snew[0]|=multi(state[0]>>(8*(3-i))&255,2) ^ multi(state[1]>>8*(3-i)&255,3) ^multi(state[2]>>8*(3-i)&255,1)^multi(state[3]>>8*(3-i)&255,1);
			if(i!=3){
				snew[0]<<=8;
			}
		}
		for(i=0;i<4;i++){
			snew[1]|=multi(state[0]>>(8*(3-i))&255,1) ^ multi(state[1]>>8*(3-i)&255,2) ^multi(state[2]>>8*(3-i)&255,3)^multi(state[3]>>8*(3-i)&255,1);
			if(i!=3){
				snew[1]<<=8;
			}
		}
		for(i=0;i<4;i++){
			snew[2]|=multi(state[0]>>(8*(3-i))&255,1) ^ multi(state[1]>>8*(3-i)&255,1) ^multi(state[2]>>8*(3-i)&255,2)^multi(state[3]>>8*(3-i)&255,3);
			if(i!=3){
				snew[2]<<=8;
			}
		}
		for(i=0;i<4;i++){
			snew[3]|=multi(state[0]>>(8*(3-i))&255,3) ^ multi(state[1]>>8*(3-i)&255,1) ^multi(state[2]>>8*(3-i)&255,1)^multi(state[3]>>8*(3-i)&255,2);
			if(i!=3){
				snew[3]<<=8;
			}
		}
		for (i = 0; i < 4; i++) {
			state[i] = snew[i];
		}
		for(i=0;i<4;i++){
			tempw[i] = key[4*j+i];
		}
		transpose(tempw);
		for(i=0;i<4;i++){
			state[i]^=tempw[i];
		}
	}
	for(i=0;i<4;i++){
		state[i] = SubWord(state[i]);
	}
	state[1] = RotWord(state[1]);
	state[2] =RotWord(RotWord(state[2]));
	state[3] = RotWord(RotWord(RotWord(state[3]))); // permute
	for(i=0;i<4;i++){
		tempw[i] = key[40+i];
	}
	transpose(tempw);
	for(i=0;i<4;i++){
		state[i]^=tempw[i];
	}
	transpose(state);
	printf("end\n");
	for(i=0;i<4;i++){
		printf("%x \n",state[i]);
	}









	return 0;
}
