#include<stddef.h>
#include "board.h"
#include "moves.h"
#include "kills.h"

#define BOARD_SIZE 8
#define BOARD_FIELDS BOARD_SIZE * BOARD_SIZE

Pawn* board[BOARD_FIELDS];
PawnColor nextMoveColor;
int restrictedRow, restrictedCol;

void restrictMovedPawn(int rfrom, int cfrom);
char isMoveRestricted();
int killPawnsAlongMove(int rfrom, int cfrom, int rto, int cto);

/**
 * Initializes the empty game board
 */
void initBoard(){
    nextMoveColor = white;
    restrictedRow = restrictedCol = -1;
    for(int i = 0; i < BOARD_FIELDS; i++){
        board[i] = NULL;
    }
}

PawnColor getNextMoveColor(){
    return nextMoveColor;
}

/**
 * Returns the board side length
 */
int getBoardSize(){
    return BOARD_SIZE;
}

/**
 * Checks whether the given position is inside the board
 * @param row The row to check
 * @param col The column to check
 */
char isInBoard(int row, int col){
    if(row < 0 || col < 0) return 0;
    if(row >= BOARD_SIZE || col >= BOARD_SIZE) return 0;

    return 1;
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
 * Restricts the pawn that can be moved to the one specified by the coordinates
 * @param rfrom The row the allowed pawn is located in
 * @param cfrom The column the allowed pawn is located in
 */
void restrictMovedPawn(int rfrom, int cfrom){
    if(rfrom == -1) cfrom = -1;
    else if(cfrom == -1) rfrom = -1;

    restrictedRow = rfrom;
    restrictedCol = cfrom;
}

/**
 * Checks if the move is restricted.
 */
char isMoveRestricted(){
    return (restrictedRow != -1) && (restrictedCol != -1);
}

/**
 * Returns a pointer to the pawn at the given position
 * @param row The row where the pawn is
 * @param col The column where the pawn is
 */
Pawn* getPawnAt(int row, int col){
    if(!isInBoard(row, col)) return NULL;

    return board[row * BOARD_SIZE + col];
}

/**
 * Places a pawn at the given position
 * @param pawn A pointer to the pawn to be placed
 * @param row The row where the pawn is to be placed
 * @param col The column where the pawn is to be placed
 */
void placePawnAt(Pawn* pawn, int row, int col){
    if(!isInBoard(row, col)) return;

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
    if(isMoveRestricted()){
        if(rfrom != restrictedRow || cfrom != restrictedCol)
            return BOARD_MUST_MOVE_ANOTHER_PAWN;
    }

    int res = checkMove(rfrom, cfrom, rto, cto);
    if(res != MOVE_LEGAL) return res;

    Pawn *p = getPawnAt(rfrom, cfrom);
    if(p == NULL) return MOVE_NO_SOURCE_PAWN;

    placePawnAt(p, rto, cto);
    placePawnAt(NULL, rfrom, cfrom);

    int kills = killPawnsAlongMove(rfrom, cfrom, rto, cto);

    // Check if there are more pawns to kill, but only if the previous move was a kill
    if(kills > 0){
        if(isPawnAbleToKill(p, rto, cto)){
            restrictMovedPawn(rto, cto);
            return BOARD_MOVE_NOT_FINISHED;
        }
    }

    PawnColor c = getPawnColor(p);
    if(c == white && rto == BOARD_SIZE - 1) transformToKing(p);
    if(c == black && rto == 0) transformToKing(p);

    nextMoveColor = !nextMoveColor;
    restrictMovedPawn(-1, -1);

    return BOARD_MOVE_SUCCESSFUL;
}

/**
 * Kills all pawns that are between (rfrom, cfrom) and (rto, cto).
 * @param rfrom The source row
 * @param cfrom The source column
 * @param rto The destination row
 * @param cto The destination column
 */
int killPawnsAlongMove(int rfrom, int cfrom, int rto, int cto){
    int rdir = 1, cdir = 1;
    if(rfrom > rto) rdir = -1;
    if(cfrom > cto) cdir = -1;

    int len = rdir * (rto - rfrom);
    Pawn* p;
    int cnt = 0;
    for(int i = 1; i < len; i++){
        p = getPawnAt(rfrom + i*rdir, cfrom + i*cdir);
        if(p == NULL) continue;

        destroyPawnAt(rfrom + i*rdir, cfrom + i*cdir);
        cnt++;
    }
    return cnt;
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

/**
 * Counts pawns of a given color that are still on the board.
 * @param color The color of pawns to count
 */
int countPawnsOfColor(PawnColor color){
    int count = 0;
    for(int row = 0; row < BOARD_SIZE; row++){
        for(int col = 0; col < BOARD_SIZE; col++){
            Pawn* p = getPawnAt(row, col);
            if(p == NULL) continue;

            if(getPawnColor(p) == color) count++;
        }
    }

    return count;
}