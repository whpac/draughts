#ifndef BOARD_H
#define BOARD_H

#include "pawn.h"

#define BOARD_MOVE_SUCCESSFUL 1000
#define BOARD_DESTINATION_UNPLAYABLE 1001
#define BOARD_DESTINATION_OCCUPIED 1002
#define BOARD_NO_SOURCE_PAWN 1003

void initBoard();
int getBoardSize();

Pawn* getPawnAt(int row, int col);
void placePawnAt(Pawn* pawn, int row, int col);
void destroyPawnAt(int row, int col);
int movePawnAtTo(int rfrom, int cfrom, int rto, int cto);

char isPlayableField(int row, int col);
void createStartLayout();

#endif