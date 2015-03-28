#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "carlos.h"

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
	Checkers::setPlayer(color);
	Checkers::setBoard(b);
	/*vector<Board> moves = Checkers::getLegalBoards(Checkers::getBoard(), color);
	if (moves.size() == 0) return LOSS;
	random_device  rand_dev;
	mt19937 generator(rand_dev());
	uniform_int_distribution<int> distr(0, moves.size() - 1);*/

	//sprintf(str, "board 15: %d", Checkers::getBoard().getPiece(15));

	Board m = mc.search(maxtime, playnow, str);
	Checkers::setBoard(m);
	
	coord c;
	short piece;
	for (int i = 1; i <= 32; i++) {
		c = Checkers::toCoord(i);
		piece = m.getPiece(i);
		if (piece == FREE) piece = 0;
		b[c.x][c.y] = piece;
	}

	sprintf(str, "%u %u %u", m.blackbit, m.whitebit, m.kingbit);

	int goal = Checkers::goalTest(Checkers::getBoard(), Checkers::getPlayer());

	return goal;
}