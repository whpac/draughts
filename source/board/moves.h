#ifndef MOVES_H
#define MOVES_H

#define MOVE_LEGAL 3000
#define MOVE_DESTINATION_UNPLAYABLE 3001
#define MOVE_DESTINATION_OCCUPIED 3002
#define MOVE_NO_SOURCE_PAWN 3003
#define MOVE_OUT_OF_BOARD 3004

int checkMove(int rfrom, int cfrom, int rto, int cto);

#endif