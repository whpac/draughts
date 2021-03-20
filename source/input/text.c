#include<stdio.h>
#include "text.h"
#include "cmd_processor.h"
#include "../board/board.h"
#include "../board/moves.h"

char doInputIteration();
int getIntArg();
void handleError();

void (*boardDisplay)();

/**
 * Begins the input loop.
 * @param board_display_func The procedure used to display the board
 */
void beginInputLoop(void (*board_display_func)()){
    boardDisplay = board_display_func;

    while(doInputIteration());
}

/**
 * Function responsible for a single input iteration.
 * Returns 0 if user requested to exit. Else, returns 1.
 */
char doInputIteration(){
    printf("Current board:\n");
    (*boardDisplay)();

    char help_char = getHelpChar();
    char cmd = help_char;
    int process_res = CMD_PROC_SUCCESSFUL;
    do{
        //if(cmd != help_char) printf("Command '%c' is unknown or cannot be completed.\n", cmd);
        handleError(process_res);
        printf("Type command ('%c' for help): ", help_char);
        scanf(" %c", &cmd);

        if(cmd == 'q') return 0;
        process_res = processCommand(cmd, &getIntArg);
    }while(process_res != CMD_PROC_SUCCESSFUL || cmd == help_char);

    return 1;
}

void handleError(int err_code){
    switch(err_code){
        case CMD_PROC_SUCCESSFUL:
            break;
        case CMD_PROC_UNKNOWN:
            printf("Passed command is unknown.\n");
            break;
        case MOVE_DESTINATION_UNPLAYABLE:
            printf("The destination field is unplayable.\n");
            break;
        case MOVE_DESTINATION_OCCUPIED:
            printf("The destination field is occupied.\n");
            break;
        case MOVE_NO_SOURCE_PAWN:
            printf("The source field contains no pawn.\n");
            break;
        case MOVE_OUT_OF_BOARD:
            printf("One of the coordinates is out of the board.\n");
            break;
        default:
            printf("An unknown error occured. Code: %d\n", err_code);
            break;
    }
}

/** Function used to read an integer argument in order to process the command */
int getIntArg(){
    int v;
    scanf("%d", &v);
    return v;
}