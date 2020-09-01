#include <iostream>
#include "perlin.h"
#include "dcurses.h"
#include <thread>
#include <vector>
#include <sys/time.h>
#include <mutex>

#include <ctime>

using namespace std;

Noise noice;

struct Point{
	int x,y;
};
int size=0;
int threads;
mutex mtx;
vector<struct Point> point;
void print_color(int idx){
	int color;
	int i=0;
	struct Point p;
	for(i=idx;i<size;i+=threads){
		p = point.at(i);
		color =232+(int)(23*noice.getColor( p.x,p.y,0.1,1));
		mtx.lock();
		dcurses_moveTo(p.x,p.y);
		dcurses_bg_pallet(color);
		printf(" ");
    	fflush(STDIN_FILENO);
		mtx.unlock();
	}
};
int main(int argc, char** argv){
	int cols,rows;
	struct timeval start,end;
	vector<thread*> point_thread;
	//argv : seed , thread num
	if(argc!=3){
		cout<<"./lab3 <seed> <number of threas>"<<endl;
		return 0;
	}


	gettimeofday(&start,NULL);
	dcurses_bounds(&cols,&rows);
	int i=0;
	int j=0;
	for(i=0;i<=cols;i++){
		for(j=0;j<=rows;j++){
			struct Point p;
			p.x = i;
			p.y=j;
			point.push_back(p);
		}
	}
	srand(time(NULL));
	noice.seed = atoi(argv[1]);
	threads = atoi(argv[2]);
	size = point.size();
	dcurses_start_fullscreen();
	dcurses_cursor(0);
    fflush(STDIN_FILENO);
	struct Point Ptemp;
	int temp=0;
	for(i=0;i<size;i++){
		temp = rand()%size;// 0~size-1
		Ptemp = point.at(temp);// change ith vector with ramdomth vector.
		point.at(temp) = point.at(i);
		point.at(i) = Ptemp;
	}//shuffle

	for(i=0;i<threads;i++){
		point_thread.push_back(new thread(print_color,i));
	}//ready to run
	for(i=0;i<point_thread.size();i++){
		thread* t = point_thread.at(i);
		t->join();
	}//run

	fflush(STDIN_FILENO);
	gettimeofday(&end,NULL);
	dcurses_reset();
	dcurses_moveTo(cols/2-9,rows/2);
	cout<<"time :"<<(end.tv_sec-start.tv_sec)*1000.0 + (end.tv_usec-start.tv_usec)/1000.0<<" msec"<<endl;
	sleep(5);
	dcurses_cursor(1);
	dcurses_end_fullscreen();
	return 0;
}



