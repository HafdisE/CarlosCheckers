#ifndef DEFINES_H
#define DEFINES_H
//logging or nay
#define LOGGING 1
//random agent or nay
#define RANDOM 0
//version number
#if !RANDOM
#define VERSION 0
#else
#define VERSION 9999
#endif
//logging path
#define PATH "C:/AI/logs/"
//db path
#define DB_PATH "C:\\AI\\db\\"

//monte carlo defines
#define INFMIN  -300000000
#define SIMULATION_LENGTH 70
#define AGGRESSIVEDB 0
#define USE_DB 1

//checkers defines
#define NW(x) (!(((x-1)/4)%2) ? x + 3 : x + 4)
#define NE(x) (!(((x-1)/4)%2) ? x + 4 : x + 5)
#define SW(x) (!(((x-1)/4)%2) ? x - 5 : x - 4)
#define SE(x) (!(((x-1)/4)%2) ? x - 4 : x - 3)

#define NORTHWEST 8
#define NORTHEAST 4
#define SOUTHWEST 2
#define SOUTHEAST 1

#define DRAW 0
#define WIN 1
#define LOSS 2
#define UNKNOWN 3

#define OCCUPIED 0
#define WHITE 1
#define BLACK 2
#define MAN 4
#define KING 8
#define FREE 16
#define CHANGECOLOR 3
#define MAXDEPTH 99
#define MAXMOVES 28


#endif