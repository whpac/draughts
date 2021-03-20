#ifndef BOARD_H
#define BOARD_H

#include "pawn.h"

void initBoard();
Pawn* getPawnAt(int row, int col);
void placePawnAt(Pawn* pawn, int row, int col);

#endif