#ifndef BOARD_H
#define BOARD_H

#include "pawn.h"

#define BOARD_MOVE_SUCCESSFUL 1000
#define BOARD_MOVE_NOT_FINISHED 1001

void initBoard();
int getBoardSize();
PawnColor getNextMoveColor();

char isInBoard(int row, int col);

Pawn* getPawnAt(int row, int col);
void placePawnAt(Pawn* pawn, int row, int col);
void destroyPawnAt(int row, int col);
int movePawnAtTo(int rfrom, int cfrom, int rto, int cto);

char isPlayableField(int row, int col);
void createStartLayout();

#endif