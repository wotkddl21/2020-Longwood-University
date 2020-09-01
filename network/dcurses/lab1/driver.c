#include "dcurses.h"
#include <stdio.h>
#include <stdlib.h>

void test_fullscreen(){
	srand(0);
	//enter full screen -- raw mode, no buffering, no echo
	dcurses_start_fullscreen();
	//turn off cursor
	dcurses_cursor(0);
	int width, height, x, y,color,background;
	dcurses_bounds(&width,&height);

	char ch = 'a';
	while(ch != 'q'){
		if(scanf("%c",&ch) == 1){
			for(int i = 0; i < 100; ++i){
			x = rand()%width;
			y = rand()%height;
			color = x/(width/7) + y/(height/7);
			color %= 7;
			color += 1;
			dcurses_fg(color);

			dcurses_moveTo(x,y);
			printf("%c",ch);
			}
		}
	}
	dcurses_end_fullscreen();
	dcurses_reset();
	dcurses_cursor(1);
}

void test_location(){
	int startX,startY;
	dcurses_location(&startX,&startY);

	for(int i =0; i < 20; ++i){
		sleep(1);
		printf("-- %2d/20 --",i+1);
		fflush(0);
		dcurses_moveTo(startX,startY);
	}
}

void test_bounds(){
	int termWidth,termHeight;
	dcurses_bounds(&termWidth,&termHeight);
	printf("\n");
	dcurses_clearScreen();
	for(int i =0; i < termHeight-1; ++i){
		if(i != 0 && i != termHeight-2){
			char bar = '$';
			if(i % 2){
				bar = '|';
			}
			printf("%c",bar);
			dcurses_moveTo(termWidth,i+1);
			printf("%c\n",bar);
		}else{
			printf("+");
			for(int j =0; j < termWidth-2; ++j){
				if(j % 2){
					printf("-");
				}else{
					printf("~");
				}
			}
			printf("+\n");
		}

	}
}

void test_attributes(char* text){
	printf("start -- %s\n",text);
	dcurses_blink(1);
    printf("blink -- %s\n",text);
	dcurses_reverse(1);
    printf("both -- %s\n",text);
	dcurses_blink(0);
    printf("reverse -- %s\n",text);
    dcurses_reverse(0);
    printf("end -- %s\n",text);
}

void test_fonts(char* text){
	for(DcursesFont font= DCURSES_FONT_NORMAL; font <= DCURSES_FONT_STRIKE; ++font){
		dcurses_setFont(font);
		printf("%s\n",text);
	}

}

void test_colors(){
	dcurses_reverse(1);
	for(DcursesColor color = DCURSES_COLOR_BLACK; color != DCURSES_COLOR_WHITE; ++color){
		dcurses_fg(color);
		printf(" ");
	}
	printf("\n");
	for(DcursesColor color = DCURSES_COLOR_BLACK; color <= DCURSES_COLOR_WHITE; ++color){
		dcurses_fg(color | DCURSES_COLOR_BRIGHT);
		printf(" ");
	}
	printf("\n");
	dcurses_reverse(0);
	dcurses_reset();
}

void test_pallet(){
	dcurses_reverse(1);

	for(int i =0; i < 256; ++i){
		dcurses_fg_pallet(i);
		printf(" ");
		if(i%16 == 0){
			printf("\n");
		}
	}
	printf("\n");
	dcurses_reverse(0);
	dcurses_reset();
}
int main(){
	test_fullscreen();
//	test_location();
//	test_bounds();
//	test_attributes("dymacek");
//	test_fonts("cmsc242");
//	test_colors();
//	test_pallet();

	return 0;
}
