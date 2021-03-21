#ifndef KILLS_H
#define KILLS_H

#include "pawn.h"

char isPawnAbleToKill(Pawn* p, int row, int col);
char isOptimalMove(Pawn* p, int rfrom, int cfrom, int rto, int cto);

#endif