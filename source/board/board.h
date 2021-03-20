#ifndef BOARD_H
#define BOARD_H

#include "pawn.h"

void initBoard();
int getBoardSize();

Pawn* getPawnAt(int row, int col);
void placePawnAt(Pawn* pawn, int row, int col);
void destroyPawnAt(int row, int col);
void movePawnAtTo(int rfrom, int cfrom, int rto, int cto);

char isPlayableField(int row, int col);
void createStartLayout();

#endif