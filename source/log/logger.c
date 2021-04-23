#include "log_action.h"
#include "logger.h"

void writeToLog(LogAction* action);

/**
 * Logs a move
 * @param rfrom The source row
 * @param cfrom The source column
 * @param rto The target row
 * @param cto The target column
 * @param player The player to associate with the move
 */
void logMove(int rfrom, int cfrom, int rto, int cto, PawnColor player){
    LogAction* la = logActionCreate();
    la->actionType = move;
    la->rfrom = rfrom;
    la->cfrom = cfrom;
    la->rto = rto;
    la->cto = cto;
    la->player = player;

    writeToLog(la);
}

/** Logs an undo action */
void logUndo(){
    LogAction* la = logActionCreate();
    la->actionType = undo;

    writeToLog(la);
}

#include<stdio.h>

/** Logs an action */
void writeToLog(LogAction* action){
    switch(action->actionType){
        case move:
            printf(
                "MOVE (%d, %d) -> (%d, %d) by %s\n",
                action->rfrom, action->cfrom, action->rto, action->cto,
                action->player == white ? "WHITE" : "BLACK"
            );
            break;
        case undo:
            printf("UNDO\n");
            break;
    }
    logActionDestroy(action);
}