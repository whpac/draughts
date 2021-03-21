#include<stdio.h>
#include "text.h"
#include "cmd_processor.h"
#include "../board/board.h"
#include "../board/moves.h"

char doInputIteration();
void handleVictory(int white_count, int black_count);
void handleError();
int getIntArg();

/** Pointer to a function responsible for displaying the game board */
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

    int white_cnt = countPawnsOfColor(white);
    int black_cnt = countPawnsOfColor(black);
    if(white_cnt == 0 || black_cnt == 0){
        handleVictory(white_cnt, black_cnt);
        return 0;
    }

    char help_char = getHelpChar();
    char cmd = help_char;
    int process_res = CMD_PROC_SUCCESSFUL;
    do{
        handleError(process_res);
        printf("Type command ('%c' for help): ", help_char);
        scanf(" %c", &cmd);

        if(cmd == 'q') return 0;
        process_res = processCommand(cmd, &getIntArg);
    }while(process_res != CMD_PROC_SUCCESSFUL || cmd == help_char);

    return 1;
}

/**
 * Handles the plight when one of the players has lost his all pawns.
 * @param white_count The number of pawns the white has
 * @param black_count The number of pawns the black has
 */
void handleVictory(int white_count, int black_count){
    if(white_count == 0){
        printf("Victory! BLACK has won the game (%d:0).\n", black_count);
    }
    if(black_count == 0){
        printf("Victory! WHITE has won the game (%d:0).\n", white_count);
    }
}

/**
 * Displays an error message appropriate to the code
 * @param err_code The error code
 */
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
        case MOVE_BACKWARDS:
            printf("Cannot move backwards.\n");
            break;
        case MOVE_MUST_BE_DIAGONAL:
            printf("Move must be along a diagonal.\n");
            break;
        case MOVE_TOO_LONG:
            printf("The move is too long.\n");
            break;
        case MOVE_CANNOT_KILL_OWN:
            printf("You cannot kill your own pawn.\n");
            break;
        case MOVE_CANNOT_MOVE_OPPONENTS_PAWN:
            printf("You cannot move your opponent's pawn.\n");
            break;
        case MOVE_TOO_MANY_OBSTACLES_FOR_KING:
            printf("There are too many obstacles for the king along this move.\n");
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