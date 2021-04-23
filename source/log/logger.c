#include<stdio.h>
#include "../data/queue.h"
#include "log_action.h"
#include "logger.h"

void writeToLog(LogAction* action);

Queue* logMessages;

/** Initializes a log */
void logInit(){
    logMessages = queueCreate();
}

/** Deinitializes a log */
void logDeinit(){
    queueDestroy(logMessages);
}

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

/**
 * Logs an action
 * @param action An action to log
 */
void writeToLog(LogAction* action){
    queuePush(logMessages, action);
}

/** Saves the log to a file */
void saveLog(){
    FILE* f = fopen("log.txt", "w");
    fputs(":: DRAUGHTS log ::", f);

    while(!queueIsEmpty(logMessages)){
        LogAction* action = queuePop(logMessages);
        switch(action->actionType){
            case move:
                fprintf(f,
                    "MOVE (%d, %d) -> (%d, %d) by %s\n",
                    action->rfrom, action->cfrom, action->rto, action->cto,
                    action->player == white ? "WHITE" : "BLACK"
                );
                break;
            case undo:
                fprintf(f, "UNDO\n");
                break;
        }
        logActionDestroy(action);
    }

    fputs(":: THE END ::", f);
    fclose(f);
}