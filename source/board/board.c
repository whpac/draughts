#include<stddef.h>
#include "board.h"

#define BOARD_SIZE 8
#define BOARD_FIELDS BOARD_SIZE * BOARD_SIZE

Pawn* board[BOARD_FIELDS];

/**
 * Initializes the empty game board
 */
void initBoard(){
    for(int i = 0; i < BOARD_FIELDS; i++){
        board[i] = NULL;
    }
}

/**
 * Returns the board side length
 */
int getBoardSize(){
    return BOARD_SIZE;
}

/**
 * Returns a pointer to the pawn at the given position
 * @param row The row where the pawn is
 * @param col The column where the pawn is
 */
Pawn* getPawnAt(int row, int col){
    if(row < 0 || col < 0) return NULL;
    if(row >= BOARD_SIZE || col >= BOARD_SIZE) return NULL;

    return board[row * BOARD_SIZE + col];
}

/**
 * Places a pawn at the given position
 * @param pawn A pointer to the pawn to be placed
 * @param row The row where the pawn is to be placed
 * @param col The column where the pawn is to be placed
 */
void placePawnAt(Pawn* pawn, int row, int col){
    if(row < 0 || col < 0) return;
    if(row >= BOARD_SIZE || col >= BOARD_SIZE) return;

    board[row * BOARD_SIZE + col] = pawn;
}

/**
 * Destroys a pawn that is places at the given position
 * @param row The row where the pawn is placed
 * @param col The column where the pawn is placed
 */
void destroyPawnAt(int row, int col){
    Pawn* p = getPawnAt(row, col);
    if(p == NULL) return;

    placePawnAt(NULL, row, col);
    destroyPawn(p);
}

/**
 * Moves a pawn across the board. It prevents from moving a pawn to
 * an unplayable field or to an occupied one.
 * @param rfrom The source row
 * @param cfrom The source column
 * @param rto The destination row
 * @param cto The destination column
 */
int movePawnAtTo(int rfrom, int cfrom, int rto, int cto){
    if(!isPlayableField(rto, cto)){
        return BOARD_DESTINATION_UNPLAYABLE;
    }

    Pawn *p = getPawnAt(rfrom, cfrom);
    if(p == NULL) return BOARD_NO_SOURCE_PAWN;

    Pawn *pto = getPawnAt(rto, cto);
    if(pto != NULL){
        return BOARD_DESTINATION_OCCUPIED;
    }

    placePawnAt(p, rto, cto);
    placePawnAt(NULL, rfrom, cfrom);

    return BOARD_MOVE_SUCCESSFUL;
}

/**
 * Checks whether the given field can host a pawn
 * @param row A coordinate of the field to check
 * @param col A coordinate of the field to check
 */
char isPlayableField(int row, int col){
    return (row + col) % 2 == 1;
}

/**
 * Places pawns in its starting positions.
 * White ones are in rows closer to 0 and black - closer to BOARD_SIZE
 */
void createStartLayout(){
    Pawn* p;

    // Place white pawns
    for(int row = 0; row < BOARD_SIZE / 2 - 1; row++){
        for(int col = 0; col < BOARD_SIZE; col++){
            if(!isPlayableField(row, col)) continue;
            p = createPawn(white, 0);
            placePawnAt(p, row, col);
        }
    }

    // Place black pawns
    for (int row = BOARD_SIZE / 2 + 1; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            if (!isPlayableField(row, col))
                continue;
            p = createPawn(black, 0);
            placePawnAt(p, row, col);
        }
    }
}