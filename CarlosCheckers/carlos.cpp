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
#if RANDOM
		sprintf(reply, "Random Carlos Montorazzo");
#else
		sprintf(reply, "Carlos Montorazzo");
#endif
		return 1;
	}

	if (strcmp(command, "about") == 0)
	{
#if RANDOM
		sprintf(reply, "Random Carlos Montorazzo 0.0.0");
#else
		sprintf(reply, "Carlos Montorazzo 0.0.0");
#endif
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

int original_color = 0;

int WINAPI getmove(int b[8][8], int color, double maxtime, char str[255], int *playnow, int info, int unused, struct CBmove *move)
{
	if (!original_color) original_color = color;
	assert(color == original_color);
	Checkers::setPlayer(color);
	Checkers::setBoard(b);


	//random
#if RANDOM
#if LOGGING
	if (!randlog.isSet()) randlog.setFile("random");
	stringstream ss;
	ss << "Starting random move select as " << (Checkers::getPlayer() == WHITE ? "white" : "black") << endl << boardToString(Checkers::getBoard()) << endl << Checkers::getBoard().blackbit << " " << Checkers::getBoard().whitebit << " " << Checkers::getBoard().kingbit;
	randlog.log("Start", ss.str());
	stringstream().swap(ss);
#endif
	vector<Board> moves = Checkers::getLegalBoards(Checkers::getBoard(), color);
	if (moves.size() == 0) return LOSS;
	random_device  rand_dev;
	mt19937 generator(rand_dev());
	uniform_int_distribution<int> distr(0, moves.size() - 1);
	int index = distr(generator);
	Board m = moves[index];
	Checkers::setBoard(m);
#if LOGGING
	ss << "Chose to do the following as " << (Checkers::getPlayer() == WHITE ? "white" : "black") << endl << boardToString(Checkers::getBoard()) << endl << Checkers::getBoard().blackbit << " " << Checkers::getBoard().whitebit << " " << Checkers::getBoard().kingbit;
	randlog.log("End", ss.str());
	stringstream().swap(ss);
#endif
	sprintf(str, "Selected move %d of %d", index+1, moves.size());
#else
	//monte carlo
	Board m = mc.search(maxtime, playnow, str);
#endif

	int goal = Checkers::goalTest(m, Checkers::getPlayer());
	if (m == Checkers::getBoard()) goal = DRAW;
	Checkers::setBoard(m);
	
	coord c;
	short piece;
	for (int i = 1; i <= 32; i++) {
		c = Checkers::toCoord(i);
		piece = m.getPiece(i);
		if (piece == FREE) piece = 0;
		b[c.x][c.y] = piece;
	}

#if !RANDOM
	sprintf(str, "%u %u %u", m.blackbit, m.whitebit, m.kingbit);
#endif

	

	return goal;
}