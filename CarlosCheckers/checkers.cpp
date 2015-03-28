#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif



/*______________________________________________________________________________

----------> name: simple checkers with enhancements
----------> author: martin fierz
----------> purpose: platform independent checkers engine
----------> version: 1.15
----------> date: 4th february 2011
----------> description: simplech.c contains a simple but fast checkers engine
and some routines to interface to checkerboard. simplech.c contains three
main parts: interface, search and move generation. these parts are
separated in the code.

board representation: the standard checkers notation is

(white)
32  31  30  29
28  27  26  25
24  23  22  21
20  19  18  17
16  15  14  13
12  11  10   9
8   7   6   5
4   3   2   1
(black)

the internal representation of the board is different, it is a
array of int with length 46, the checkers board is numbered
like this:

(white)
37  38  39  40
32  33  34  35
28  29  30  31
23  24  25  26
19  20  21  22
14  15  16  17
10  11  12  13
5   6   7   8
(black)

let's say, you would like to teach the program that it is
important to keep a back rank guard. you can for instance
add the following (not very sophisticated) code for this:

if(b[6] & (BLACK|MAN)) eval++;
if(b[8] & (BLACK|MAN)) eval++;
if(b[37] & (WHITE|MAN)) eval--;
if(b[39] & (WHITE|MAN)) eval--;

the evaluation function is seen from the point of view of the
black player, so you increase the value v if you think the
position is good for black.

simple checkers is free for anyone to use, in any way, explicitly also
in commercial products without the need for asking me. Naturally, I would
appreciate if you tell me that you are using it, and if you acknowledge
my contribution to your project.

questions, comments, suggestions to:

Martin Fierz
checkers@fierz.ch


/*----------> includes */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <random>     /* srand, rand */
#include "carlos.h"
#include "checkers.h"




/*-------------- PART 1: dll stuff -------------------------------------------*/

BOOL WINAPI DllEntryPoint(HANDLE hDLL, DWORD dwReason, LPVOID lpReserved)
{
	/* in a dll you used to have LibMain instead of WinMain in windows programs, or main
	in normal C programs
	win32 replaces LibMain with DllEntryPoint.*/

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		/* dll loaded. put initializations here! */
		break;
	case DLL_PROCESS_DETACH:
		/* program is unloading dll. put clean up here! */
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	default:
		break;
	}
	return TRUE;
}

int  WINAPI enginecommand(char str[256], char reply[256])
{
	// answer to commands sent by CheckerBoard.
	// Simple Checkers does not answer to some of the commands,
	// eg it has no engine options.

	char command[256], param1[256], param2[256];

	sscanf(str, "%s %s %s", command, param1, param2);

	// check for command keywords 

	if (strcmp(command, "name") == 0)
	{
		sprintf(reply, "Carlos Montorazzo");
		return 1;
	}

	if (strcmp(command, "about") == 0)
	{
		sprintf(reply, "Carlos Montorazzo 0.0.0");
		return 1;
	}

	if (strcmp(command, "help") == 0)
	{
		sprintf(reply, "simplechhelp.htm");
		return 1;
	}

	if (strcmp(command, "set") == 0)
	{
		if (strcmp(param1, "hashsize") == 0)
		{
			sprintf(reply, "?");
			return 0;
		}
		if (strcmp(param1, "book") == 0)
		{
			sprintf(reply, "?");
			return 0;
		}
	}

	if (strcmp(command, "get") == 0)
	{
		if (strcmp(param1, "hashsize") == 0)
		{
			sprintf(reply, "?");
			return 0;
		}
		if (strcmp(param1, "book") == 0)
		{
			sprintf(reply, "?");
			return 0;
		}
		if (strcmp(param1, "protocolversion") == 0)
		{
			sprintf(reply, "2");
			return 1;
		}
		if (strcmp(param1, "gametype") == 0)
		{
			sprintf(reply, "21");
			return 1;
		}
	}
	sprintf(reply, "?");
	return 0;
}

int WINAPI getmove(int b[8][8], int color, double maxtime, char str[255], int *playnow, int info, int unused, struct CBmove *move)
{
	Checkers::setBoard(b);
	vector<CBmove2> moves = Checkers::getLegalMoves(color);
	if (moves.size() == 0) return DRAW;
	random_device  rand_dev;
	mt19937 generator(rand_dev());
	std::uniform_int_distribution<int> distr(0, moves.size() - 1);
	int index = distr(generator);
	CBmove2 m = moves[index];
	Checkers::applyMove(m);
	coord c;
	short piece;
	for (int i = 1; i <= 32; i++) {
		c = Checkers::toCoord(i);
		piece = Checkers::getBoard().getPiece(i);
		//if(piece & KING) sprintf(str, "piece is a king");
		if (piece == FREE) piece = 0;
		b[c.x][c.y] = piece;
	}

	sprintf(str, "I am %s. Move %d of %s %d,%d to %s %d,%d out of %d possible", (color == WHITE ? "white" : "black"), index + 1, (m.oldpiece & MAN ? "man" : "king"), m.from.x, m.from.y, (m.newpiece & MAN ? "man" : "king"), m.to.x, m.to.y, moves.size());

	return Checkers::goalTest(Checkers::getBoard(), color);
}