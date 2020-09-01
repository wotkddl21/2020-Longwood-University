#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include "dcurses.h"
//Enter full screen mode
//disable echo and buffered input
static struct termios stored;
void dcurses_start_fullscreen(){
	struct termios new;
	tcgetattr(0,&stored);
	new = stored;
	cfmakeraw(&new);
	new.c_lflag |=ISIG;
	tcsetattr(0,TCSANOW,&new);
	dcurses_clearScreen();
	return;
};


//return to the previous mode
void dcurses_end_fullscreen(){
	dcurses_clearScreen();
	printf("\e[1;1H");
	tcsetattr(0,TCSANOW,&stored);
};


//clear the screen
void dcurses_clearScreen(){
	printf("\e[2J");
};


//set the current font
void dcurses_setFont(DcursesFont font){
	if(font==DCURSES_FONT_NORMAL){
		printf("\e[0m");
	}
	else if(font==DCURSES_FONT_ITALICS){
		printf("\e[3m");
	}
	else if(font==DCURSES_FONT_BOLD){
		printf("\e[1m");
	}
	else if(font==DCURSES_FONT_FAINT){
		printf("\e[2m");
	}
	else if(font==DCURSES_FONT_UNDERLINE){
		printf("\e[4m");
	}
	else if(font==DCURSES_FONT_STRIKE){
		printf("\e[9m");
	}
};

//set the forground color to 3/4 bit color
void dcurses_fg(DcursesColor color){
	if(color==DCURSES_COLOR_BLACK){
		printf("\e[30m");
	}
	else if(color==DCURSES_COLOR_RED){
		printf("\e[31m");
	}
	else if(color==DCURSES_COLOR_GREEN){
		printf("\e[32m");
	}
	else if(color==DCURSES_COLOR_YELLOW){
		printf("\e[33m");
	}
	else if(color==DCURSES_COLOR_BLUE){
		printf("\e[34m");
	}
	else if(color==DCURSES_COLOR_MAGENTA){
		printf("\e[35m");
	}
	else if(color==DCURSES_COLOR_CYAN){
		printf("\e[36m");
	}
	else if(color==DCURSES_COLOR_WHITE){
		printf("\e[37m");
	}
	
	else if(color==(DCURSES_COLOR_BLACK|DCURSES_COLOR_BRIGHT)){
		printf("\e[90m");
	}
	else if(color==(DCURSES_COLOR_RED|DCURSES_COLOR_BRIGHT)){
		printf("\e[91m");
	}
	else if(color==(DCURSES_COLOR_GREEN|DCURSES_COLOR_BRIGHT)){
		printf("\e[92m");
	}
	else if(color==(DCURSES_COLOR_YELLOW|DCURSES_COLOR_BRIGHT)){
		printf("\e[93m");
	}
	else if(color==(DCURSES_COLOR_BLUE|DCURSES_COLOR_BRIGHT)){
		printf("\e[94m");
	}
	else if(color==(DCURSES_COLOR_MAGENTA|DCURSES_COLOR_BRIGHT)){
		printf("\e[95m");
	}
	else if(color==(DCURSES_COLOR_CYAN|DCURSES_COLOR_BRIGHT)){
		printf("\e[96m");
	}
	else if(color==(DCURSES_COLOR_WHITE|DCURSES_COLOR_BRIGHT)){
		printf("\e[97m");
	}
};

//set the background color to 3/4 bit color
void dcurses_bg(DcursesColor color){

	if(color==DCURSES_COLOR_BLACK){
		printf("\e[40m");
	}
	else if(color==DCURSES_COLOR_RED){
		printf("\e[41m");
	}
	else if(color==DCURSES_COLOR_GREEN){
		printf("\e[42m");
	}
	else if(color==DCURSES_COLOR_YELLOW){
		printf("\e[43m");
	}
	else if(color==DCURSES_COLOR_BLUE){
		printf("\e[44m");
	}
	else if(color==DCURSES_COLOR_MAGENTA){
		printf("\e[45m");
	}
	else if(color==DCURSES_COLOR_CYAN){
		printf("\e[46m");
	}
	else if(color==DCURSES_COLOR_WHITE){
		printf("\e[47m");
	}
	
	else if(color==(DCURSES_COLOR_BLACK|DCURSES_COLOR_BRIGHT)){
		printf("\e[100m");
	}
	else if(color==(DCURSES_COLOR_RED|DCURSES_COLOR_BRIGHT)){
		printf("\e[101m");
	}
	else if(color==(DCURSES_COLOR_GREEN|DCURSES_COLOR_BRIGHT)){
		printf("\e[102m");
	}
	else if(color==(DCURSES_COLOR_YELLOW|DCURSES_COLOR_BRIGHT)){
		printf("\e[103m");
	}
	else if(color==(DCURSES_COLOR_BLUE|DCURSES_COLOR_BRIGHT)){
		printf("\e[104m");
	}
	else if(color==(DCURSES_COLOR_MAGENTA|DCURSES_COLOR_BRIGHT)){
		printf("\e[105m");
	}
	else if(color==(DCURSES_COLOR_CYAN|DCURSES_COLOR_BRIGHT)){
		printf("\e[106m");
	}
	else if(color==(DCURSES_COLOR_WHITE|DCURSES_COLOR_BRIGHT)){
		printf("\e[107m");
	}
};

//set the background color to a pallet color 0-255
void dcurses_bg_pallet(int color){

	printf("\e[48;5;%dm",color);
};

//set the foreground color to a pallet color 0-255
void dcurses_fg_pallet(int color){
	printf("\e[38;5;%dm",color);
};

//enable/disable cursor
void dcurses_cursor(int val){
	if(val){
		printf("\e[?25h");
	}
	else{
		printf("\e[?25l");
	}
};

//enable / disable blink
void dcurses_blink(int val){
	if(val){
		printf("\e[5m");
	}
	else{
		printf("\e[0m");
	}
};

//enable/ disable reverse
void dcurses_reverse(int val){
	if(val){
		printf("\e[7m");
	}
	else{
		printf("\e[0m");
	}
};

//reset attributes ( fonts and colors )
void dcurses_reset(){
	printf("\e[0m");
};

//get the screen bounds
void dcurses_bounds(int* width, int* height){
	
	dcurses_moveTo(1000,1000);
	dcurses_location(width,height);
//	dcurses_moveTo(*width,*height);

};

//get the cursor location
void dcurses_location(int* x,int* y){
	//printf("\e[%d;%ds",x,y);
	struct termios new;
	struct termios loc;
	tcgetattr(0,&loc);
	new = loc;
	cfmakeraw(&new);
	tcsetattr(0,TCSANOW,&new);
	printf("\e[6n");
	scanf("\e[%d;%dR",y,x);
	tcsetattr(0,TCSANOW,&loc);

};

//move the cursor to the location
void dcurses_moveTo(int x,int y){
	printf("\e[%d;%dH",y,x);
};
