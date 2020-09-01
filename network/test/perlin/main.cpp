#include <iostream>
#include "perlin.h"
#include "dcurses.h"
#include <thread>
#include <vector>
#include <sys/time.h>
#include <mutex>

using namespace std;

Noise noice;

struct Point{
	int x,y;
};


int main(int argc, char** argv){
	int cols,rows;

	dcurses_bounds(&cols,&rows);

	Point c = {cols/2,rows/2};

	noice.seed = 12345;
	dcurses_start_fullscreen();
    fflush(STDIN_FILENO);

	dcurses_cursor(0);
	int color = 232+(int)(23*noice.getColor(c.x,c.y,0.1,1));
	dcurses_moveTo(c.x,c.y);
	dcurses_bg_pallet(color);
	printf(" ");
	fflush(STDIN_FILENO);

	sleep(5);
	dcurses_cursor(1);
	dcurses_end_fullscreen();
	return 0;
}
