#include<stddef.h>
#include "moves.h"
#include "board.h"
#include "pawn.h"

int checkOnlyForward(Pawn* p, int rfrom, int cfrom, int rto, int cto);
int checkProperDistance(Pawn* p, int rfrom, int cfrom, int rto, int cto);
int checkKingObstacles(Pawn* p, int rfrom, int cfrom, int rto, int cto);

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

    chk = checkKingObstacles(p, rfrom, cfrom, rto, cto);
    if(chk != MOVE_LEGAL) return chk;

    return MOVE_LEGAL;
}

/**
 * Calculates the move length. If the move is not diagonal, returns -1.
 * @param rfrom The source row
 * @param cfrom The source column
 * @param rto The destination row
 * @param cto The destination column
 */
int getMoveLength(int rfrom, int cfrom, int rto, int cto){
    int dr = rfrom - rto;
    int dc = cfrom - cto;

    if(dr < 0) dr *= -1;
    if(dc < 0) dc *= -1;

    if(dr != dc) return -1;
    return dr;
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
    int len = getMoveLength(rfrom, cfrom, rto, cto);

    if(len == -1) return MOVE_MUST_BE_DIAGONAL;
    if(isPawnKing(p)) return MOVE_LEGAL;

    if(len == 1) return MOVE_LEGAL;
    if(len == 2){
        Position* killed_pawn = getPawnAlongMove(rfrom, cfrom, rto, cto);
        Pawn* killed = positionGetPawn(killed_pawn);
        positionDestroy(killed_pawn);

        if(killed == NULL) return MOVE_TOO_LONG;
        if(getPawnColor(killed) == getPawnColor(p)) return MOVE_CANNOT_KILL_OWN;
        return MOVE_LEGAL;
    }

    return MOVE_TOO_LONG;
}

/**
 * Checks obstacles along a king's move. An obstacle is: pawn of the same color as 
 * the king or more than one pawn of the other color (even if separated by a blank field). 
 * The separation is considered an obstacle, because killing two pawns requires two consecutive moves.
 * @param p The pawn that is being moved
 * @param rfrom The source row
 * @param cfrom The source column
 * @param rto The destination row
 * @param cto The destination column
 */
int checkKingObstacles(Pawn* p, int rfrom, int cfrom, int rto, int cto){
    if(!isPawnKing(p)) return MOVE_LEGAL;

    int rdir = 1, cdir = 1;
    if(rfrom > rto) rdir = -1;
    if(cfrom > cto) cdir = -1;

    int len = rdir * (rto - rfrom);
    int opponents_met = 0;
    for(int i = 1; i < len; i++){
        Pawn* p_mid = getPawnAt(rfrom + i*rdir, cfrom + i*cdir);
        if(p_mid == NULL) continue;

        if(getPawnColor(p_mid) == getPawnColor(p)) return MOVE_CANNOT_KILL_OWN;
        else opponents_met++;

        if(opponents_met > 1) return MOVE_TOO_MANY_OBSTACLES_FOR_KING;
    }

    return MOVE_LEGAL;
}

/**
 * Returns the pawn that is placed in the middle of move. If it happens that 
 * there are more pawns that one, the last is returned. However, according to the rules, 
 * there will always be only one pawn along a killing move.
 * @param rfrom The source row
 * @param cfrom The source column
 * @param rto The target row
 * @param cto The target column
 */
Position* getPawnAlongMove(int rfrom, int cfrom, int rto, int cto){
    int rdir = 1, cdir = 1;
    if(rfrom > rto) rdir = -1;
    if(cfrom > cto) cdir = -1;

    int len = rdir * (rto - rfrom);
    Pawn* p;
    Position* pos = positionCreate(-1, -1, NULL);

    for(int i = 1; i < len; i++){
        int curr_r = rfrom + i*rdir;
        int curr_c = cfrom + i*cdir;

        p = getPawnAt(curr_r, curr_c);
        if(p == NULL) continue;

        positionDestroy(pos);
        pos = positionCreate(curr_r, curr_c, p);
    }
    return pos;
}