#ifndef MOVES_H
#define MOVES_H

#include "position.h"

#define MOVE_LEGAL 3000
#define MOVE_DESTINATION_UNPLAYABLE 3001
#define MOVE_DESTINATION_OCCUPIED 3002
#define MOVE_NO_SOURCE_PAWN 3003
#define MOVE_OUT_OF_BOARD 3004
#define MOVE_BACKWARDS 3005
#define MOVE_MUST_BE_DIAGONAL 3006
#define MOVE_TOO_LONG 3007
#define MOVE_CANNOT_KILL_OWN 3008
#define MOVE_CANNOT_MOVE_OPPONENTS_PAWN 3009
#define MOVE_TOO_MANY_OBSTACLES_FOR_KING 3010

int checkMove(int rfrom, int cfrom, int rto, int cto);
int getMoveLength(int rfrom, int cfrom, int rto, int cto);
Position* getPawnAlongMove(int rfrom, int cfrom, int rto, int cto);

#endif