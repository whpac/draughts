#include<stdio.h>
#include "cmd_processor.h"
#include "../../board/board.h"
#include "display.h"

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
    }else if(cmd == 'u'){
        undoMove();
    }
    else return CMD_PROC_UNKNOWN;
    return CMD_PROC_SUCCESSFUL;
}

/** Prints the help message */
void printHelp(){
    displayHeader();
    printf("DRAUGHTS commands help\n");
    printf("  %c\n    invokes help\n", getHelpChar());
    printf("  m \033[3mrow_from col_from row_to col_to\033[0m\n");
    printf("    moves pawn from given coordinates to those given by the second pair\n");
    printf("  %c\n    quits the game\n", getQuitChar());
    printf("  u\n    undoes the last move\n");
    printf("\n\n");

    printf("Press Enter to close");
    while(getchar() != '\n');
}

/**
 * Moves a pawn across the board
 * @param rfrom The source row
 * @param cfrom The source column
 * @param rto The destination row
 * @param cto The destination column
 */
int movePawn(int rfrom, int cfrom, int rto, int cto){
    int res = attemptMovePawnAtTo(rfrom, cfrom, rto, cto);
    if(res == BOARD_MOVE_SUCCESSFUL || res == BOARD_MOVE_NOT_FINISHED) return CMD_PROC_SUCCESSFUL;
    return res;
}