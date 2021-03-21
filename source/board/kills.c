#include<stddef.h>
#include "kills.h"
#include "board.h"
#include "moves.h"

char isManAbleToKill(int row, int col);
char isKingAbleToKill(Pawn* p, int row, int col);

/**
 * Checks whether the pawn is able to kill any pawn on the board
 * @param p The pawn to be checked
 * @param row The row the pawn is located in
 * @param col The column the pawn is located in
 */
char isPawnAbleToKill(Pawn* p, int row, int col){
    if(isPawnKing(p)) return isKingAbleToKill(p, row, col);
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
 * @param p The king to be checked
 * @param row The row the pawn is located in
 * @param col The column the pawn is located in
 */
char isKingAbleToKill(Pawn* p, int row, int col){
    int rdir[4] = {1, 1, -1, -1};
    int cdir[4] = {1, -1, 1, -1};

    int opponents_met, crow, ccol;
    for(int j = 0; j < 4; j++){
        opponents_met = 0;
        for(int i = 1;; i++){
            crow = row + i*rdir[j];
            ccol = col + i*cdir[j];
            if(!isInBoard(crow, ccol)) break;

            Pawn* pb = getPawnAt(crow, ccol);

            // If this is an empty field behind a opponent's pawn, it's possible to kill
            if(pb == NULL){
                if(opponents_met > 0) return 1;
                else continue;
            }

            // If the first pawn is not an opponent's one - this path leads to nothing
            if(getPawnColor(p) == getPawnColor(pb)) break;

            // Count the met opponent's pawn. If it's the second one - the killing is impossible here
            opponents_met++;
            if(opponents_met >= 2) break;
        }
    }
    return 0;
}