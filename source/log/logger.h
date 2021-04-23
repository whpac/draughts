#ifndef LOGGER_H
#define LOGGER_H

#include "../board/pawn.h"

void logInit();
void logDeinit();

void logMove(int rfrom, int cfrom, int rto, int cto, PawnColor player);
void logUndo();

void saveLog();

#endif