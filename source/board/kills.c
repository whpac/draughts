#include<stddef.h>
#include "kills.h"
#include "board.h"
#include "moves.h"
#include "position.h"

char isManAbleToKill(Pawn* p, int row, int col);
char isKingAbleToKill(Pawn* p, int row, int col);
int countPossibleKillsByMan(Pawn* p, int row, int col, int rfrom, int cfrom);

/**
 * Checks whether the pawn is able to kill any pawn on the board
 * @param p The pawn to be checked
 * @param row The row the pawn is located in
 * @param col The column the pawn is located in
 */
char isPawnAbleToKill(Pawn* p, int row, int col){
    if(isPawnKing(p)) return isKingAbleToKill(p, row, col);
    else return isManAbleToKill(p, row, col);
}

/**
 * Checks whether the man is able to kill any pawn on the board
 * @param p The man to be checked
 * @param row The row the pawn is located in
 * @param col The column the pawn is located in
 */
char isManAbleToKill(Pawn* p, int row, int col){
    int rdir[4] = {1, 1, -1, -1};
    int cdir[4] = {1, -1, 1, -1};

    for(int i = 0; i < 4; i++){
        Pawn* killed = getPawnAt(row + rdir[i], col + cdir[i]);

        if(killed == NULL) continue;
        if(getPawnColor(killed) == getPawnColor(p)) continue;

        if(!isInBoard(row + 2*rdir[i], col + 2*cdir[i])) continue;

        Pawn* behind_killed = getPawnAt(row + 2*rdir[i], col + 2*cdir[i]);
        if(behind_killed == NULL) return 1;
    }
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

// TODO: Replace these ints with a tree of fields
// ! Seems to be deprecated. Use the version using tree when ready
/**
 * Checks if the move is an optimal one. Optimal means the most kills.
 * @param p The killing pawn
 * @param rfrom The source row
 * @param cfrom The source column
 * @param rto The destination row
 * @param cto The destination column
 */
char isOptimalMove(Pawn* p, int rfrom, int cfrom, int rto, int cto){
    int move_kills = countPossibleKillsByMan(p, rfrom, cfrom, -1, -1);

    for(int r = 0; r < getBoardSize(); r++){
        for(int c = 0; c < getBoardSize(); c++){
            if(!isPlayableField(r, c)) continue;

            Pawn* f_pawn = getPawnAt(r, c);
            if(f_pawn == NULL) continue;
            if(getPawnColor(p) != getPawnColor(f_pawn)) continue;

            int k = countPossibleKillsByMan(f_pawn, r, c, -1, -1);
            if(k > move_kills) return 0;
        }
    }

    return 1;
}

/**
 * Counts how many pawns can be killed in a series by a man standing at (rfrom, cfrom)
 * @param p The pawn that is to kill others
 * @param row The source row
 * @param col The source column
 * @param rfrom The previous call's row. In the outermost scope should be -1 or row
 * @param cfrom The previous call's column. In the outermost scope should be -1 or col
 */
int countPossibleKillsByMan(Pawn* p, int row, int col, int rfrom, int cfrom){
    if(!isInBoard(row, col)) return 0;
    if(!isManAbleToKill(p, row, col)) return 0;

    int rdir[4] = {1, 1, -1, -1};
    int cdir[4] = {1, -1, 1, -1};

    int kills = 0;
    for(int i = 0; i < 4; i++){
        int crow = row + 2*rdir[i];
        int ccol = col + 2*cdir[i];

        // Skip the previous field to avoid infinite loops
        if(crow == rfrom && ccol == cfrom) continue;
        if(!isInBoard(crow, ccol)) continue;

        // The target field must be empty
        Pawn* target = getPawnAt(crow, ccol);
        if(target != NULL) continue;

        // Cannot kill air or own pawns
        Pawn* killed = getPawnAt(row + rdir[i], col + cdir[i]);
        if(killed == NULL) continue;
        if(getPawnColor(p) == getPawnColor(killed)) continue;

        int res = 1 + countPossibleKillsByMan(p, crow, ccol, row, col);
        if(res > kills) kills = res;
    }

    return kills;
}


/**
 * Checks all possible moves and returns a list of these which are allowed
 * @param color The player's color whose move it is
 */
void getAllowedMoves(PawnColor color){

}

/**
 * Returns all allowed paths from the given field
 * @param p The pawn being moved
 * @param row The source row
 * @param col The source column
 */
TreeNode* getAllowedKillsFrom(Pawn* p, int row, int col){
    int rdir[] = {1, 1, -1, -1};
    int cdir[] = {1, -1, 1, -1};

    TreeNode* moves_tree = treeCreate(positionCreate(row, col, p));
    int max_depth = 0;

    for(int i = 0; i < 4; i++){
        int longest_move = isPawnKing(p) ? (getBoardSize() - 1) : 2;
        for(int j = 2; j <= longest_move; j++){
            int rto = row + j*rdir[i];
            int cto = col + j*cdir[i];
            if(checkMove(row, col, rto, cto) == MOVE_LEGAL){
                // An allowed kill must actually be a kill
                Position* killed_pos = getPawnAlongMove(row, col, rto, cto);
                Pawn* killed_pawn = positionGetPawn(killed_pos);
                positionDestroy(killed_pos);
                if(killed_pawn == NULL) continue;

                // Move the pawn and perform a recursive check
                int res = movePawnAtTo(p, row, col, rto, cto);
                if(res != BOARD_MOVE_SUCCESSFUL && res != BOARD_MOVE_NOT_FINISHED) continue;

                TreeNode* moves_subtree = getAllowedKillsFrom(p, rto, cto);
                undoMove();                 // There is no need to undo a failed move

                // Add a tree only if it is of at least the same depth as others
                // If the new subtree's depth is greater, remove former subtrees
                int subtree_depth = treeGetDepth(moves_subtree);
                if(subtree_depth > max_depth){
                    max_depth = subtree_depth;
                    treeDestroy(moves_tree, 1);
                    moves_tree = treeCreate(positionCreate(row, col, p));
                    treeAddChildNode(moves_tree, moves_subtree);
                }else if(subtree_depth == max_depth){
                    treeAddChildNode(moves_tree, moves_subtree);
                }else{
                    treeDestroy(moves_subtree, 1);
                }
            }
        }
    }

    return moves_tree;
}
