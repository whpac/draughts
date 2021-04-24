#include<stdio.h>
#include "../data/queue.h"
#include "log_action.h"
#include "logger.h"

#define FILE_NAME_LENGTH 16

void writeToLog(LogAction* action);
void clearLogQueue();

Queue* logMessages;
char outputFile[FILE_NAME_LENGTH + 1];

/** Initializes a log */
void logInit(){
    logMessages = queueCreate();
    for(int i = 0; i <= FILE_NAME_LENGTH; i++) outputFile[i] = '\0';
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
 * Logs a game over situation
 * @param winner The winner
 */
void logGameOver(PawnColor winner){
    LogAction* la = logActionCreate();
    la->actionType = over;
    la->player = winner;

    writeToLog(la);
}

/**
 * Logs an action
 * @param action An action to log
 */
void writeToLog(LogAction* action){
    queuePush(logMessages, action);
}

/**
 * Sets the target file name
 * @param name The file name
 */
void setLogFileName(char* name){
    for(int i = 0; i < FILE_NAME_LENGTH; i++){
        outputFile[i] = name[i];
        if(name[i] == '\0') break;
    }
}

/** Saves the log to a file */
void saveLog(){
    if(outputFile[0] == '\0'){
        // Clear log without saving
        clearLogQueue();
        return;
    }

    FILE* f = fopen(outputFile, "w");

    if(f == NULL){
        printf("Unable to open the log file '%s'\n", outputFile);
        clearLogQueue();
        return;
    }

    fputs(":: DRAUGHTS log ::\n", f);

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
                fputs("UNDO\n", f);
                break;
            case over:
                fprintf(f, "OVER %s has won\n", action->player == white ? "WHITE" : "BLACK");
                break;
        }
        logActionDestroy(action);
    }

    fputs(":: THE END ::", f);
    fclose(f);
}

/** Removes all the log messages */
void clearLogQueue(){
    while(!queueIsEmpty(logMessages)){
        logActionDestroy(queuePop(logMessages));
    }
}