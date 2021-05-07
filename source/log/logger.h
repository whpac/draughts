#ifndef LOGGER_H
#define LOGGER_H

#include "../board/pawn.h"

typedef int (*MoveFunction)(int rfrom, int cfrom, int rto, int cto);
typedef void (*UndoFunction)();

void logInit();
void logDeinit();

void logMove(int rfrom, int cfrom, int rto, int cto, PawnColor player);
void logUndo();
void logGameOver(PawnColor winner);

void setLogFileName(char* name);
int saveLog();
int readLog(MoveFunction move, UndoFunction undo);

#endif