#include<stdio.h>
#include<string.h>
#include "../board/board.h"
#include "../data/queue.h"
#include "log_action.h"
#include "logger.h"

#define FILE_NAME_LENGTH 16

void writeToLog(LogAction* action);
void clearLogQueue();

Queue* logMessages;
char logFile[FILE_NAME_LENGTH + 1];

/** Initializes a log */
void logInit(){
    logMessages = queueCreate();
    for(int i = 0; i <= FILE_NAME_LENGTH; i++) logFile[i] = '\0';
}

/** Deinitializes a log */
void logDeinit(){
    clearLogQueue();
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
    for(int i = 0; i <= FILE_NAME_LENGTH; i++) logFile[i] = '\0';
    for(int i = 0; i < FILE_NAME_LENGTH; i++){
        logFile[i] = name[i];
        if(name[i] == '\0') break;
    }
}

/** Saves the log to a file. Returns 0 on success */
int saveLog(){
    if(logFile[0] == '\0'){
        // If file is empty, return
        return 0;
    }

    FILE* f = fopen(logFile, "w");

    if(f == NULL){
        // printf("Unable to open the log file '%s'\n", logFile);
        return LOGGER_CANNOT_OPEN_FILE;
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
                fputs("UNDO MOVE\n", f);
                break;
            case over:
                fprintf(f, "OVER %s has won\n", action->player == white ? "WHITE" : "BLACK");
                break;
        }
        logActionDestroy(action);
    }

    fputs(":: THE END ::", f);
    fclose(f);
    return 0;
}

/** Removes all the log messages */
void clearLogQueue(){
    while(!queueIsEmpty(logMessages)){
        logActionDestroy(queuePop(logMessages));
    }
}

/**
 * Reads a log file. Returns 0 on success
 * @param move A function to call in order to move a pawn
 * @param undo A function to call in order to undo a move
 */
int readLog(MoveFunction move, UndoFunction undo){
    if(logFile[0] == '\0') return 0;

    FILE* f = fopen(logFile, "r");

    if(f == NULL){
        // printf("Unable to open the file '%s'\n", logFile);
        setLogFileName("");
        return LOGGER_CANNOT_OPEN_FILE;
    }

    // Prevents from accidental overwriting the log
    setLogFileName("");

    char signature[] = ":: DRAUGHTS log ::\n";
    for(int i = 0; signature[i] != '\0'; i++){
        char c = fgetc(f);
        if(signature[i] != c){
            // printf("The specified file is not valid\n");
            fclose(f);
            return LOGGER_FILE_INVALID;
        }
    }

    while(!feof(f)){
        char action[8];
        fscanf(f, " %7s%", &action);

        if(action[0] != ':'){
            if(!strcmp(action, "MOVE")){
                int rfrom, cfrom, rto, cto;
                fscanf(f, " (%d, %d) -> (%d, %d)", &rfrom, &cfrom, &rto, &cto);

                int res = BOARD_MOVE_NOT_OPTIMAL;
                if(move != NULL) res = move(rfrom, cfrom, rto, cto);

                if(res != BOARD_MOVE_SUCCESSFUL && res != BOARD_MOVE_NOT_FINISHED){
                    printf("An invalid move encountered. Stopping.\n");
                    break;
                }
            }else if(!strcmp(action, "UNDO")){
                if(undo != NULL) undo();
            }
            // No need to do anything if action is unknown
        }

        // Read the rest of the line
        while(fgetc(f) != '\n' && !feof(f));
    }

    fclose(f);
    return 0;
}