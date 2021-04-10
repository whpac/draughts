#ifndef KILLS_H
#define KILLS_H

#include "pawn.h"
#include "../data/tree.h"
#include "../data/list.h"

char isPawnAbleToKill(Pawn* p, int row, int col);
TreeNode* getAllowedKillsFrom(Pawn* p, int row, int col);
List* getAllowedKills(PawnColor color);
List* getAllowedNonKillingMoves(PawnColor color);

#endif