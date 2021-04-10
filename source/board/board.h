#ifndef BOARD_H
#define BOARD_H

#include "pawn.h"

#define BOARD_MOVE_SUCCESSFUL 1000
#define BOARD_MOVE_NOT_FINISHED 1001
#define BOARD_MUST_MOVE_ANOTHER_PAWN 1002
#define BOARD_MOVE_NOT_OPTIMAL 1003

void initBoard();
int getBoardSize();
PawnColor getNextMoveColor();

char isInBoard(int row, int col);
char isPlayableField(int row, int col);

Pawn* getPawnAt(int row, int col);
void placePawnAt(Pawn* pawn, int row, int col);
int attemptMovePawnAtTo(int rfrom, int cfrom, int rto, int cto);
int movePawnAtTo(Pawn* p, int rfrom, int cfrom, int rto, int cto);
void undoMove();

void createStartLayout();
int countPawnsOfColor(PawnColor color);

void destroyBoard();

#endif