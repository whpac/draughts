#ifndef KILLS_H
#define KILLS_H

#include "pawn.h"
#include "../data/tree.h"

char isPawnAbleToKill(Pawn* p, int row, int col);
char isOptimalMove(Pawn* p, int rfrom, int cfrom, int rto, int cto);
TreeNode* getAllowedKillsFrom(Pawn* p, int row, int col);

#endif