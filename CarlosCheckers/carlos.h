#ifndef CARLOS_H
#define CARLOS_H

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <random>
#include "montecarlo.h"



/*----------> compile options  */
#undef MUTE
#undef VERBOSE
#define STATISTICS




/*----------> structure definitions  */
struct move2
{
	short n;
	int m[8];
};
struct coor             /* coordinate structure for board coordinates */
{
	int x;
	int y;
};

struct CBmove            	/* all the information you need about a move */
{
	int ismove;          /* kind of superfluous: is 0 if the move is not a valid move */
	int newpiece;        /* what type of piece appears on to */
	int oldpiece;        /* what disappears on from */
	struct coor from, to; /* coordinates of the piece - in 8x8 notation!*/
	struct coor path[12]; /* intermediate path coordinates of the moving piece */
	struct coor del[12]; /* squares whose pieces are deleted after the move */
	int delpiece[12];    /* what is on these squares */
} GCBmove;


MonteCarlo mc;

BOOL WINAPI DllEntryPoint(HANDLE hDLL, DWORD dwReason, LPVOID lpReserved);

/* required functions */
extern "C" {
	int WINAPI getmove(int b[8][8], int color, double maxtime, char str[255], int *playnow, int info, int unused, struct CBmove *move);
	int  WINAPI enginecommand(char str[256], char reply[256]);
}

#endif
