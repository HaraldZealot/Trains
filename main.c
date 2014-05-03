#define _XOPEN_SOURCE_EXTENDED
#define _POSIX_C_SOURCE	199309L

#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <time.h>

static const char block[4] = {0342, 0226, 0210, 0};
static const char thinLine[4] = {0342, 0224, 0200, 0};
static const char thickLine[4] = {0342, 0224, 0201, 0};
static const char startMutex[4] = {0342, 0225, 0276, 0};
static const char closeSemaphore[4] = {0342, 0227, 0217, 0};
static const char openSemaphore[4] = {0342, 0227, 0211, 0};



enum Colors{black,normal,green,red};
//ESC=27

const int railVerticalPosition = 5;
const int railMutexSegment = 13;
const int countOfSegment = 3;
int railLength;

void sigWinch(int signo);

void initialiseProgram();

int currentCirclePosition(int position, int circleLength);
int nextCirclePosition(int position, int circleLength);

void eraseRail(int row, int col);

int main()
{
	initialiseProgram();

	railLength = countOfSegment * railMutexSegment;

	attron(COLOR_PAIR(green));
	for(int i = 0; i < countOfSegment; ++i)
	{
		move(railVerticalPosition-2, i * railMutexSegment +1);
		printw("%s", openSemaphore);
	}
	attroff(COLOR_PAIR(green));
	for(int i=0; i < railLength; ++i)
	{
		move(railVerticalPosition, i);
		printw("%s", i % railMutexSegment? thinLine : startMutex);
	}
	refresh();
	
	int trainHead=2;
	int trainTail=0;
	for(int i=trainTail; i < trainHead; ++i)
	{
		move(railVerticalPosition, i);
		printw("%s",block);
	}
	refresh();
	
	while(1)
	{
		move(railVerticalPosition, nextCirclePosition(trainHead, railLength));
		printw("%s",block);
		eraseRail(railVerticalPosition, currentCirclePosition(trainTail, railLength));
		refresh();
		struct timespec s;
		s.tv_sec = 0;
		s.tv_nsec = 50000000L;
		nanosleep(&s, NULL);
		trainHead = nextCirclePosition(trainHead, railLength);
		trainTail = nextCirclePosition(trainTail, railLength);
	}
	

	endwin();
	return 0;
}

int currentCirclePosition(int position, int circleLength)
{
	return position;
}

int nextCirclePosition(int position, int circleLength)
{
	return (position + 1) % circleLength;
}

void eraseRail(int row, int col)
{
	if(row == railVerticalPosition && col >=0 && col < railLength)
	{
		move(row, col);
		printw("%s", col % railMutexSegment? thinLine : startMutex);
	}
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
