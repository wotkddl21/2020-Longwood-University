#include <iostream>
#include <iomanip>
#include <assert.h>
#include <arpa/inet.h>

using namespace std;

typedef unsigned int block[16]; //A block is 512 bits or 16 words long
typedef unsigned int digest[5]; //A digest is 160 bits or 5 bytes long.

unsigned int K[80];
unsigned int initial_values[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};

void pad(string line, block*& message, int& n)
{
	//Store the length as a 64-bit integer
	unsigned long long int l = line.length();
	
	//Every character in the string is 8 bits long
	unsigned long long int length = l*8; 

	//Padding will add at least an additional 512 - (len%512) bits
	unsigned long long int pad_length = length + 512 - (length % 512); 

	//If there were already 448 bits, though, we need an additional block for the extra 0's
	if ((length % 512) >= 448) {
		pad_length += 512;
	}
	
	//The number of blocks is number of bits divided by 512
	n = pad_length / 512; 
	
	//Message is an "n" block string
	message = new block[n];	

	//Which we can treat as one very long array of bytes
	unsigned char * bytes = (unsigned char *)message;
	
	//Copy the text into the message array
	unsigned int pos = 0;
	while (pos < l) {
			assert(pos < n*64);
			bytes[pos] = line.at(pos);
			++pos;
	}

	//Append a 1 bit followed by at least 7 zeros 
	//(since our input consists of bytes, we know we need at least 8 bits)
	assert(pos < n*64);
	bytes[pos] = 0x80; 
	++pos;

	//Pad with 0's until length % 512 is 448 bits (448 bits/8 = 56 bytes)
	while (pos % 64 != 56) {
		assert(pos < n*64);
		bytes[pos] = 0x00;
		++pos;
	}

	//Pad with 64-bit length of the integer.  Must be done carefully to avoid endianness issues
	unsigned long long temp = length;
	for (int i=7; i >=0; --i)
	{
		bytes[pos+i]=temp&0xff;
		temp>>=8;
	}
	pos += 8;
	assert(pos*8 == n*512);
}

void sha(block* message, int n, digest& output)
{
		//Set H to the initial values as defined in FIPS 180-1
		unsigned int H[5];
		for (int i=0; i < 5; ++i) {
			H[i] = initial_values[i];
		}
	unsigned int temp;
		//For each block
		for (unsigned int i=0; i < n; ++i) {
			//Split the block into 16 bytes named "W"
			unsigned int W[80];
			for (int j=0; j < 16; ++j) {
				W[j] = htonl(message[i][j]);
			}		
			for (int j = 16; j < 80; j++) {
				 temp = (W[j - 3] ^ W[j - 8] ^ W[j - 14] ^ W[j - 16]);
				W[j] = temp << 1 | temp >> 31;
			}
			//Initialize A, B, C, D, and E to H[0]...H[4]
			unsigned int A = H[0];
			unsigned int B = H[1];
			unsigned int C = H[2];
			unsigned int D = H[3];
			unsigned int E = H[4];

			//Implement algorithm here
			for (i = 0; i < 20; i++) {
				temp = (A << 5 | A >> 27) + E + W[i] + K[i] + ((B&C) | ((~B)&D));
				E = D; D = C; C = (B << 30 | B >> 2); B = A; A = temp;
			}
			for (i = 20; i < 40; i++) {

				temp = (A << 5 | A >> 27) + E + W[i] + K[i] + (B^C^D);
				E = D; D = C; C = (B << 30 | B >> 2); B = A; A = temp;

			}
			for (i = 40; i < 60; i++) {

				temp = (A << 5 | A >> 27) + E + W[i] + K[i] + ((B&C) | (B&D) | (C&D));
				E = D; D = C; C = (B << 30 | B >> 2); B = A; A = temp;

			}
			for (i = 60; i < 80; i++) {

				temp = (A << 5 | A >> 27) + E + W[i] + K[i] + (B^C^D);
				E = D; D = C; C = (B << 30 | B >> 2); B = A; A = temp;

			}






			


			//Spin the "tumblers" by incrementing H[i] by the appropriate amount
			H[0] += A; H[1] += B; H[2] += C; H[3] += D; H[4] += E;
		}
		
		//After processing last block, our output is the values of the H's
		output[0] = H[0]; output[1] = H[1]; output[2] = H[2]; output[3] = H[3]; output[4] = H[4];
}

int main()
{
		string line; //Input string

		int n; //Number of 512-bit blocks
		block* message; //Input in 512-bit blocks
		digest output;  //Output as a 160-bit digest
		
		//Set up Constants
		for (unsigned int i=0; i < 80; ++i) {
				K[i] = i < 20 ? 0x5A827999 : i < 40 ? 0x6ED9EBA1 : i < 60 ? 0x8F1BBCDC : 0xCA62C1D6;
		}

		//Read one line, pad it, hash it, and output the hash
		while (getline(cin, line)) {
			pad(line, message, n);
			sha(message, n, output);

			//Print the hash as 5 32-bit words (160-bits)
			unsigned int *bytes = (unsigned int *)output;
			for (int i=0; i < 5; ++i) {
					cout << hex << setw(8) << setfill('0') << bytes[i];
			}
			cout << endl;
		}
		return 0;
}
