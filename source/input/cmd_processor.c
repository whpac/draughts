#include<stdio.h>
#include "cmd_processor.h"
#include "../board/board.h"

void printHelp();
int movePawn(int rfrom, int cfrom, int rto, int cto);

/** Returns the character that invokes help. */
char getHelpChar(){
    return '?';
}

/** Returns the character that breaks the input loop. */
char getQuitChar(){
    return 'q';
}

/**
 * Processes the command. Returns 0 if command was successful.
 * @param cmd The command to process
 * @param int_getter Function used to get value of an integer argument
 */
int processCommand(char cmd, int (*int_getter)()){
    if(cmd == getHelpChar()) printHelp();
    else if(cmd == 'm'){
        int rfrom = (*int_getter)();
        int cfrom = (*int_getter)();
        int rto = (*int_getter)();
        int cto = (*int_getter)();
        return movePawn(rfrom, cfrom, rto, cto);
    }
    else return CMD_PROC_UNKNOWN;
    return CMD_PROC_SUCCESSFUL;
}

/** Prints the help message */
void printHelp(){
    printf("DRAUGHTS commands help\n");
    printf("  %c\n    invokes help\n", getHelpChar());
    printf("  m (row_from) (col_from) (row_to) (col_to)\n");
    printf("    moves pawn from given coordinates to those given by the second pair\n");
    printf("  %c\n    quits the game\n", getQuitChar());
    printf("\n");
}

/**
 * Moves a pawn across the board
 * @param rfrom The source row
 * @param cfrom The source column
 * @param rto The destination row
 * @param cto The destination column
 */
int movePawn(int rfrom, int cfrom, int rto, int cto){
    int res = movePawnAtTo(rfrom, cfrom, rto, cto);
    if(res == BOARD_MOVE_SUCCESSFUL) return CMD_PROC_SUCCESSFUL;
    return res;
}