#ifndef POSITION_H
#define POSITION_H

#include "pawn.h"

typedef struct Position Position;

Position* positionCreate(int row, int col, Pawn* pawn);
void positionDestroy(Position* pos);
int positionGetRow(Position* pos);
int positionGetColumn(Position* pos);
Pawn* positionGetPawn(Position* pos);

#endif