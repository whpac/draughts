#include<stdio.h>
#include "input.h"
#include "cmd_processor.h"
#include "display.h"
#include "../../board/board.h"
#include "../../board/moves.h"
#include "../../log/logger.h"

char doInputIteration();
void handleVictory(int white_count, int black_count);
void handleError();
int getIntArg();
void readFromFile();
void saveToFile();

/**
 * Begins the input loop.
 */
void cliBeginInputLoop(){
    displayHeader();

    printf("Do you want to load a saved game? (y/N): ");
    printf("\033[2mN\033[0m\033[1D");    // Prints a gray N and goes back by one letter

    char c;
    scanf("%c", &c);
    if(c != '\n') while(getchar() != '\n');

    if(c == 'y' || c == 'Y') readFromFile();

    while(doInputIteration());

    printf("\nDo you want to save this game? (y/N): ");
    printf("\033[2mN\033[0m\033[1D");    // Prints a gray N and goes back by one letter
    scanf("%c", &c);

    if(c != 'y' && c != 'Y') return;
    if(c != '\n') while(getchar() != '\n');

    printf("\033[1A\033[2K");   // Clear last line
    saveToFile();

    printf("\033[2K");
}

/**
 * Function responsible for a single input iteration.
 * Returns 0 if user requested to exit. Else, returns 1.
 */
char doInputIteration(){
    displayHeader();
    displayBoard();

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
        scanf("%c", &cmd);

        if(cmd == 'q'){
            while(getchar() != '\n');
            return 0;
        }
        process_res = processCommand(cmd, &getIntArg);
        while(getchar() != '\n');  // Removes any newlines from the buffer
    }while(process_res != CMD_PROC_SUCCESSFUL);

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
    printf("\033[1;31m");   // Print error in red
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
        case BOARD_MUST_MOVE_ANOTHER_PAWN:
            printf("You have to move a different pawn.\n");
            break;
        case BOARD_MOVE_NOT_OPTIMAL:
            printf("There's another move with more kills.\n");
            break;
        default:
            printf("An unknown error occured. Code: %d\n", err_code);
            break;
    }
    printf("\033[0m");   // Reset to default font style
}

/** Function used to read an integer argument in order to process the command */
int getIntArg(){
    int v;
    scanf("%d", &v);
    return v;
}

/** Asks the user to type the source file name and reads it */
void readFromFile(){
    printf("\033[1A\033[2K");   // Clear last line
    while(1){
        printf("Type the file path: ");

        char file_name[65];
        file_name[64] = '\0';

        // Read input char-by-char to detect empty input
        for(int i = 0; i < 63; i++){
            if((file_name[i] = getchar()) == '\n'){
                file_name[i] = '\0';
                break;
            }
        }
        if(file_name[0] == '\0') break;

        setLogFileName(file_name);
        int res = readLog(attemptMovePawnAtTo, undoMove);
        if(!res) break;

        printf("\033[2K\033[1;31m");
        switch(res){
            case LOGGER_CANNOT_OPEN_FILE:
                printf("Cannot open file '%s'.", file_name);
                break;
            case LOGGER_FILE_INVALID:
                printf("File '%s' is invalid.", file_name);
                break;
            default:
                printf("An error occured.");
                break;
        }
        printf("\033[0m\n");

        printf("\033[2K\033[2mTip: leave empty to start a new game.\033[0m\n");
        printf("\033[1A\033[1A\033[1A\033[2K");
    }
}

/** Asks user for target file name and saves the game there */
void saveToFile(){
    while(1){
        // Ask for file to save
        printf("Type the target file path: ");

        char file_name[65];
        file_name[64] = '\0';

        // Read input char-by-char to detect empty input
        for(int i = 0; i < 63; i++){
            if((file_name[i] = getchar()) == '\n'){
                file_name[i] = '\0';
                break;
            }
        }
        if(file_name[0] == '\0') break;

        setLogFileName(file_name);
        if(!saveLog()) break;

        printf("\033[2K\033[1;31mCannot open file '%s'.\033[0m\n", file_name);
        printf("\033[2K\033[2mTip: leave empty to discard the game.\033[0m\n");
        printf("\033[1A\033[1A\033[1A\033[2K");
    }
}
