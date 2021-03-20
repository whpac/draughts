#include<stdio.h>
#include "text.h"
#include "cmd_processor.h"

char doInputIteration();
int getIntArg();

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
    char process_res;
    do{
        if(cmd != help_char) printf("Unknown command: '%c'.\n", cmd);
        printf("Type command ('%c' for help): ", help_char);
        scanf(" %c", &cmd);

        if(cmd == 'q') return 0;
        process_res = processCommand(cmd, &getIntArg);
    }while(!process_res || cmd == help_char);

    return 1;
}

/** Function used to read an integer argument in order to process the command */
int getIntArg(){
    int v;
    scanf("%d", &v);
    return v;
}