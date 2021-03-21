#include "kills.h"
#include "moves.h"

char isManAbleToKill(int row, int col);
char isKingAbleToKill(int row, int col);

/**
 * Checks whether the pawn is able to kill any pawn on the board
 * @param p The pawn to be checked
 * @param row The row the pawn is located in
 * @param col The column the pawn is located in
 */
char isPawnAbleToKill(Pawn* p, int row, int col){
    if(isPawnKing(p)) return isKingAbleToKill(row, col);
    else return isManAbleToKill(row, col);
}

/**
 * Checks whether the man is able to kill any pawn on the board
 * @param row The row the pawn is located in
 * @param col The column the pawn is located in
 */
char isManAbleToKill(int row, int col){
    if(checkMove(row, col, row+2, col+2) == MOVE_LEGAL) return 1;
    if(checkMove(row, col, row+2, col-2) == MOVE_LEGAL) return 1;
    if(checkMove(row, col, row-2, col+2) == MOVE_LEGAL) return 1;
    if(checkMove(row, col, row-2, col-2) == MOVE_LEGAL) return 1;
    return 0;
}

/**
 * Checks whether the king is able to kill any pawn on the board
 * @param row The row the pawn is located in
 * @param col The column the pawn is located in
 */
char isKingAbleToKill(int row, int col){
    return 0;
}