#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "montecarlo.h"

#define OCCUPIED 0
#define WHITE 1
#define BLACK 2
#define MAN 4
#define KING 8
#define FREE 16
#define CHANGECOLOR 3
#define MAXDEPTH 99
#define MAXMOVES 28

/*----------> compile options  */
#undef MUTE
#undef VERBOSE
#define STATISTICS


/* return values */
#define DRAW 0
#define WIN 1
#define LOSS 2
#define UNKNOWN 3

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


BOOL WINAPI DllEntryPoint(HANDLE hDLL, DWORD dwReason, LPVOID lpReserved);

/* required functions */
extern "C" {
	int WINAPI getmove(int b[8][8], int color, double maxtime, char str[255], int *playnow, int info, int unused, struct CBmove *move);
	int  WINAPI enginecommand(char str[256], char reply[256]);
}
