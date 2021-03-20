#include<stddef.h>
#include "moves.h"
#include "board.h"

/**
 * Checks whether a move (rfrom, cfrom) -> (rto, cto) is legal.
 * @param rfrom The source row
 * @param cfrom The source column
 * @param rto The destination row
 * @param cto The destination column
 */
int checkMove(int rfrom, int cfrom, int rto, int cto){
    if(!isInBoard(rfrom, cfrom)) return MOVE_OUT_OF_BOARD;
    if(!isInBoard(rto, cto)) return MOVE_OUT_OF_BOARD;

    if(!isPlayableField(rto, cto)){
        return MOVE_DESTINATION_UNPLAYABLE;
    }

    Pawn *p = getPawnAt(rfrom, cfrom);
    if (p == NULL) return MOVE_NO_SOURCE_PAWN;

    Pawn *pto = getPawnAt(rto, cto);
    if(pto != NULL) return MOVE_DESTINATION_OCCUPIED;

    return MOVE_LEGAL;
}