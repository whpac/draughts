#include<stddef.h>
#include "moves.h"
#include "board.h"
#include "pawn.h"

int checkOnlyForward(Pawn* p, int rfrom, int cfrom, int rto, int cto);
int checkProperDistance(Pawn* p, int rfrom, int cfrom, int rto, int cto);

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

    if(getNextMoveColor() != getPawnColor(p)) return MOVE_CANNOT_MOVE_OPPONENTS_PAWN;

    int chk = checkOnlyForward(p, rfrom, cfrom, rto, cto);
    if(chk != MOVE_LEGAL) return chk;

    chk = checkProperDistance(p, rfrom, cfrom, rto, cto);
    if(chk != MOVE_LEGAL) return chk;

    return MOVE_LEGAL;
}

/**
 * Checks if the move is forward for the given pawn color. This analysis 
 * is done only for moves that are one short jump long. What's more, 
 * kings are extempt from the check - they can move backwards.
 * @param p The pawn that is being moved
 * @param rfrom The source row
 * @param cfrom The source column
 * @param rto The destination row
 * @param cto The destination column
 */
int checkOnlyForward(Pawn* p, int rfrom, int cfrom, int rto, int cto){
    if(isPawnKing(p)) return MOVE_LEGAL;

    // Long moves are considered legal because it's possible to kill pawns backwards.
    // Only one direction is checked because both must be the same. This is checked in checkProperDistance().
    int dr = rfrom - rto;
    if(dr < -1 || dr > 1) return MOVE_LEGAL;

    PawnColor c = getPawnColor(p);
    if(c == white){
        return rfrom < rto ? MOVE_LEGAL : MOVE_BACKWARDS;
    }else{
        return rfrom > rto ? MOVE_LEGAL : MOVE_BACKWARDS;
    }
}

/**
 * Checks if the move length is appropriate to the pawn type. Also checks if 
 * the path (rfrom, cfrom) -> (rto, cto) is a proper diagonal.
 * @param p The pawn that is being moved
 * @param rfrom The source row
 * @param cfrom The source column
 * @param rto The destination row
 * @param cto The destination column
 */
int checkProperDistance(Pawn* p, int rfrom, int cfrom, int rto, int cto){
    int dr = rfrom - rto;
    int dc = cfrom - cto;

    if(dr < 0) dr *= -1;
    if(dc < 0) dc *= -1;

    if(dr != dc) return MOVE_MUST_BE_DIAGONAL;
    if(isPawnKing(p)) return MOVE_LEGAL;

    if(dr == 1) return MOVE_LEGAL;
    if(dr == 2){
        int rkill = (rfrom + rto) / 2;
        int ckill = (cfrom + cto) / 2;
        Pawn* killed = getPawnAt(rkill, ckill);

        if(killed == NULL) return MOVE_TOO_LONG;
        if(getPawnColor(killed) == getPawnColor(p)) return MOVE_CANNOT_KILL_OWN;
        return MOVE_LEGAL;
    }

    return MOVE_TOO_LONG;
}