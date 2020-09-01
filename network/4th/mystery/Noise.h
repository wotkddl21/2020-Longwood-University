#ifndef NOISE__H
#define NOISE__H
#include <cmath>
#include <random>

using namespace std;


struct Noise{
	int seed;
	double p = 0.5;
	double numOctives = 512;

//http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
	double cosineInterpolate(double a,double b, double w){
		double f = (1-cos(w*M_PI))/2;
		return  a*(1-f) + b*f;
	}

//http://burtleburtle.net/bob/hash/integer.html 4 byte hash function
	int hash32( unsigned int a){
		a = (a+0x7ed55d16) + (a<<12);
		a = (a^0xc761c23c) ^ (a>>19);
		a = (a+0x165667b1) + (a<<5);
		a = (a+0xd3a2646c) ^ (a<<9);
		a = (a+0xfd7046c5) + (a<<3);
		a = (a^0xb55a4f09) ^ (a>>16);
		return a;
	}

//convert 3 ints into a "random" number 3 levels of hashing plus the seed as an offset
//make a double as a
	double noise(int x, int y,int z){
		return (double)(hash32(seed+(z + hash32(hash32(x + hash32(y))))))/(double)RAND_MAX;
	}


//http://mrl.nyu.edu/~perlin/noise/
//and
//http://freespace.virgin.net/hugo.elias/models/m_perlin.htm

	double interpolateNoise(double x,double y,double z){
		int X = int(x);
		int Y = int(y);
		int Z = int(z);

		double y1 = cosineInterpolate(noise(X,Y,  Z),noise(X+1,Y,  Z),x-X);
		double y2 = cosineInterpolate(noise(X,Y+1,Z),noise(X+1,Y+1,Z),x-X);

		double y3 = cosineInterpolate(noise(X,Y,  Z+1),noise(X+1,Y,  Z+1),x-X);
		double y4 = cosineInterpolate(noise(X,Y+1,Z+1),noise(X+1,Y+1,Z+1),x-X);

		double z5 = cosineInterpolate(y1,y2,y-Y);
		double z6 = cosineInterpolate(y3,y4,y-Y);

		return cosineInterpolate(z5,z6,z-Z);
	}


//http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
//Generate octaves of noise
//we use the same noise function for each level just chainging the frequency
// returns a doubleing point value between -1 and -1
	double octaves(double x, double y,double z) {
		double value = 0;
		double frequency = 1;
		double amplitude = 1;
		double max = 0;

		for(int i =0 ; i< numOctives; i++){

			value += interpolateNoise(frequency*x, frequency*y, frequency*z) * amplitude;
			max += amplitude;
			frequency *= 2;
			amplitude *= p;
		}

		return value/max;
	}

	double getColor(double x,double y,double z,int slow){
		//find noise at point x,y and z
		double total = 0;
		double num = 0;
		for(int i = 0-slow; i <= slow; ++i){
			for(int j = 0-slow; j <= slow; ++j){
				total += octaves((x+i)/32,(y+j)/32,z)/2.0 + 0.5;
				num += 1;
			}
		}
		return total/num;
	}

};

#endif
