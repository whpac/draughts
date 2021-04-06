#include<stddef.h>
#include<malloc.h>

#include "board.h"
#include "moves.h"
#include "kills.h"
#include "position.h"
#include "../data/stack.h"
#include "../data/tree.h"

#define BOARD_SIZE 8
#define BOARD_FIELDS (BOARD_SIZE * BOARD_SIZE)

typedef struct MoveDescriptor {
    Pawn* movedPawn;
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;
    char wasPawnKing;
    char wasMoveRestricted;
    Pawn* killedPawn;
    int killedRow;
    int killedCol;
} MoveDescriptor;

Pawn* board[BOARD_FIELDS];
PawnColor nextMoveColor;
int restrictedRow, restrictedCol;
Stack* movesHistory;

void restrictMovedPawn(int rfrom, int cfrom);
char isMoveRestricted();
Position* killPawnAlongMove(int rfrom, int cfrom, int rto, int cto);

/**
 * Initializes the empty game board
 */
void initBoard(){
    nextMoveColor = white;
    restrictedRow = restrictedCol = -1;
    movesHistory = stackCreate();
    for(int i = 0; i < BOARD_FIELDS; i++){
        board[i] = NULL;
    }
}

/** Returns color of a player that's going to perform the next move. */
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
 * @deprecated
 */
/*
! Deprecated. Since killed pawns are stored in move history, they cannot be removed from the memory
void destroyPawnAt(int row, int col){
    Pawn* p = getPawnAt(row, col);
    if(p == NULL) return;

    placePawnAt(NULL, row, col);
    destroyPawn(p);
}
*/

/**
 * Attempts to move a pawn across the board. It prevents from moving a pawn to 
 * an unplayable field or to an occupied one. What's more, it does a check whether 
 * the attempted move is optimal.
 * @param rfrom The source row
 * @param cfrom The source column
 * @param rto The destination row
 * @param cto The destination column
 */
int attemptMovePawnAtTo(int rfrom, int cfrom, int rto, int cto){
    Pawn *p = getPawnAt(rfrom, cfrom);
    if(p == NULL) return MOVE_NO_SOURCE_PAWN;

    if(isMoveRestricted()){
        if(rfrom != restrictedRow || cfrom != restrictedCol)
            return BOARD_MUST_MOVE_ANOTHER_PAWN;

        char is_legal = 0;
        TreeNode* allowed_moves = getAllowedKillsFrom(p, rfrom, cfrom);
        for(int i = 0; i < treeGetChildNodesCount(allowed_moves); i++){
            TreeNode* child_node = treeGetChildNode(allowed_moves, i);
            Position* to_pos = treeGetNodeContent(child_node);

            if(rto == positionGetRow(to_pos) && cto == positionGetColumn(to_pos)){
                is_legal = 1;
                break;
            }
        }
        treeDestroy(allowed_moves, 1);

        if(!is_legal)
            return BOARD_MOVE_NOT_OPTIMAL;
    }else{
        char is_legal = 0;
        List* allowed_kills_list = getAllowedKills(getNextMoveColor());
        int allowed_kills_length = listGetLength(allowed_kills_list);

        for(int i = 0; i < allowed_kills_length && !is_legal; i++){
            TreeNode* moves = listGet(allowed_kills_list, i);
            Position* from = treeGetNodeContent(moves);

            if(rfrom == positionGetRow(from) && cfrom == positionGetColumn(from)){
                for(int i = 0; i < treeGetChildNodesCount(moves); i++){
                    TreeNode* child_node = treeGetChildNode(moves, i);
                    Position* to_pos = treeGetNodeContent(child_node);

                    if(rto == positionGetRow(to_pos) && cto == positionGetColumn(to_pos)){
                        is_legal = 1;
                        break;
                    }
                }
            }
        }

        if(allowed_kills_length == 0) is_legal = 1;

        for(int i = 0; i < allowed_kills_length; i++){
            TreeNode* n = listGet(allowed_kills_list, 0);
            treeDestroy(n, 1);
            listRemove(allowed_kills_list, 0, 0);
        }
        listDestroy(allowed_kills_list, 0);

        if(!is_legal)
            return BOARD_MOVE_NOT_OPTIMAL;
    }
    return movePawnAtTo(p, rfrom, cfrom, rto, cto);
}

/**
 * Moves a pawn across the board. It prevents from moving a pawn to 
 * an unplayable field or to an occupied one. Doesn't check if the move is optimal. 
 * attemptMovePawnAtTo() should be used to process user's requests.
 * @param p The pawn being moved
 * @param rfrom The source row
 * @param cfrom The source column
 * @param rto The destination row
 * @param cto The destination column
 */
int movePawnAtTo(Pawn* p, int rfrom, int cfrom, int rto, int cto){
    int res = checkMove(rfrom, cfrom, rto, cto);
    if(res != MOVE_LEGAL) return res;

    MoveDescriptor* md = malloc(sizeof(MoveDescriptor));
    md->movedPawn = p;
    md->fromRow = rfrom;
    md->fromCol = cfrom;
    md->toRow = rto;
    md->toCol = cto;
    md->wasPawnKing = isPawnKing(p);
    md->wasMoveRestricted = isMoveRestricted();

    placePawnAt(p, rto, cto);
    placePawnAt(NULL, rfrom, cfrom);

    stackPush(movesHistory, md);

    Position* killed_pawn = killPawnAlongMove(rfrom, cfrom, rto, cto);
    md->killedPawn = positionGetPawn(killed_pawn);
    md->killedRow = positionGetRow(killed_pawn);
    md->killedCol = positionGetColumn(killed_pawn);
    positionDestroy(killed_pawn);
    killed_pawn = NULL;

    // Check if there are more pawns to kill, but only if the previous move was a kill
    if(md->killedPawn != NULL){
        if(isPawnAbleToKill(p, rto, cto)){
            restrictMovedPawn(rto, cto);
            return BOARD_MOVE_NOT_FINISHED;
        }
    }

    PawnColor c = getPawnColor(p);
    if(c == white && rto == BOARD_SIZE - 1) setIsPawnKing(p, 1);
    if(c == black && rto == 0) setIsPawnKing(p, 1);

    nextMoveColor = !nextMoveColor;
    restrictMovedPawn(-1, -1);

    return BOARD_MOVE_SUCCESSFUL;
}

/**
 * Kills a pawn that is between (rfrom, cfrom) and (rto, cto). 
 * According to the rules and move checker, there is always at most one pawn killed during the move. 
 * That's why the returned value is not an array or other list. 
 * Returns the killed pawn and its position.
 * @param rfrom The source row
 * @param cfrom The source column
 * @param rto The destination row
 * @param cto The destination column
 */
Position* killPawnAlongMove(int rfrom, int cfrom, int rto, int cto){
    Position* killed = getPawnAlongMove(rfrom, cfrom, rto, cto);
    int killed_row = positionGetRow(killed);
    int killed_col = positionGetColumn(killed);

    if(killed_col >= 0 && killed_row >= 0){
        placePawnAt(NULL, killed_row, killed_col);
    }
    return killed;
}

/** Discards the recently made move */
void undoMove(){
    MoveDescriptor* md = stackPop(movesHistory);
    if(md == NULL) return;

    placePawnAt(md->movedPawn, md->fromRow, md->fromCol);
    placePawnAt(NULL, md->toRow, md->toCol);
    setIsPawnKing(md->movedPawn, md->wasPawnKing);
    placePawnAt(md->killedPawn, md->killedRow, md->killedCol);

    if(md->wasMoveRestricted){
        restrictMovedPawn(md->fromRow, md->fromCol);
    }else{
        restrictMovedPawn(-1, -1);
    }

    nextMoveColor = getPawnColor(md->movedPawn);

    // This move descriptor is not useful anymore.
    free(md);
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