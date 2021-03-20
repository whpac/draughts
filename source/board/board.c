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