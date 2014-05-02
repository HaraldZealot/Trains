#define _XOPEN_SOURCE_EXTENDED

#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <locale.h>

static const char block[4] = {0342, 0226, 0210, 0};
static const char thinLine[4] = {0342, 0224, 0200, 0};
static const char thickLine[4] = {0342, 0224, 0201, 0};
static const char closeSemaphore[4] = {0342, 0227, 0217, 0};
static const char openSemaphore[4] = {0342, 0227, 0211, 0};



enum Colors{black,normal,green,red};
//ESC=27

void sigWinch(int signo);

void initialiseProgram();

int main()
{
	initialiseProgram();
	int railPosition = 5;
	int railLength=13;

	attron(COLOR_PAIR(green));
	move(railPosition-2, 2);
	printw("%s", openSemaphore);
	attroff(COLOR_PAIR(green));
	for(int i=1; i<=railLength; ++i)
	{
		move(railPosition, i);
		printw("%s",thinLine);
	}
	refresh();
	
	int trainHead=2;
	int trainTail=0;
	for(int i=trainTail; i <= trainHead; ++i)
	{
		move(railPosition, i + 1);
		printw("%s",block);
	}
	refresh();
	
	while(1)
	{
		move(railPosition, trainHead + 2);
		printw("%s",block);
		move(railPosition, trainTail + 1);
		printw("%s",thinLine);
		refresh();
		trainHead = (trainHead + 1) % railLength;
		trainTail = (trainTail + 1) % railLength;
	}
	

	endwin();
	return 0;
}

void initialiseProgram()
{
	setlocale(LC_ALL, "");
	initscr();
	signal (SIGWINCH, sigWinch);	
	if(has_colors() == FALSE)
	{
		endwin();
		fprintf(stderr, "no colors\n");
		exit(1);
	}
	cbreak();
	noecho();
	curs_set(0);
	start_color();
	init_pair(black, COLOR_BLACK, COLOR_BLACK);
	init_pair(normal, COLOR_WHITE, COLOR_BLACK);
	init_pair(green, COLOR_GREEN, COLOR_BLACK);
	init_pair(red, COLOR_RED, COLOR_BLACK);
}

void sigWinch(int signo)
{
	struct winsize size;
	ioctl( fileno( stdout ), TIOCGWINSZ, ( char * ) & size );
	resizeterm( size.ws_row, size.ws_col );
}
