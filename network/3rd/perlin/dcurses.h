#ifndef DCURSES__H
#define DCURSES__H
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

//Constants for 8 and 16 color mode
typedef enum {
 DCURSES_COLOR_BLACK   = 0,
 DCURSES_COLOR_RED     = 1,
 DCURSES_COLOR_GREEN   = 2,
 DCURSES_COLOR_YELLOW  = 3,
 DCURSES_COLOR_BLUE    = 4,
 DCURSES_COLOR_MAGENTA = 5,
 DCURSES_COLOR_CYAN    = 6,
 DCURSES_COLOR_WHITE   = 7,
 DCURSES_COLOR_BRIGHT  = 0xFF00
} DcursesColor;

//Constants for fonts
typedef enum {
 DCURSES_FONT_NORMAL,
 DCURSES_FONT_ITALICS,
 DCURSES_FONT_BOLD,
 DCURSES_FONT_FAINT,
 DCURSES_FONT_UNDERLINE,
 DCURSES_FONT_STRIKE
} DcursesFont;

//Enter full screen mode
//disable echo and buffered input
void dcurses_start_fullscreen();
//return to the previous mode
void dcurses_end_fullscreen();

//clear the screen
void dcurses_clearScreen();

//set the current font
void dcurses_setFont(DcursesFont font);


//set the foreground color to 3/4 bit color
void dcurses_fg(DcursesColor color );
//se the background color to 3/4 bit color
void dcurses_bg(DcursesColor color );

//set the background color to a pallet color 0-255
void dcurses_bg_pallet(int color);
//set the foreground color to a pallet color 0-255
void dcurses_fg_pallet(int color);

//enable/disable cursor
void dcurses_cursor(int val);

//enable/disable blink
void dcurses_blink(int val);

//enable/disable reverse
void dcurses_reverse(int val);

//reset attributes? just fonts and colors? whatever reset does
void dcurses_reset();

//get the screen bounds
void dcurses_bounds(int* width,int* height);

//get the cursor location
void dcurses_location(int* x,int* y);

//move the cursor to the location
void dcurses_moveTo(int x,int y);

#endif
