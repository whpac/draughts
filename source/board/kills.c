#include<stddef.h>
#include "kills.h"
#include "board.h"
#include "moves.h"
#include "position.h"

char isManAbleToKill(Pawn* p, int row, int col);
char isKingAbleToKill(Pawn* p, int row, int col);
TreeNode* getAllowedMovesFrom(Pawn* p, int row, int col);

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

/**
 * Checks all possible kills and returns a list of these which are allowed. 
 * The returned structure is a list of TreeNode's.
 * @param color The player's color whose move it is
 */
List* getAllowedKills(PawnColor color){
    List* allowed_kills = listCreate();
    int max_kill_length = 1;

    for(int row = 0; row < getBoardSize(); row++){
        for(int col = 0; col < getBoardSize(); col++){
            // Check if the source pawn is valid
            Pawn* p = getPawnAt(row, col);
            if(p == NULL) continue;
            if(getPawnColor(p) != color) continue;

            // Get legal kills from the source positions
            TreeNode* kills_from_pos = getAllowedKillsFrom(p, row, col);
            int kill_length = treeGetDepth(kills_from_pos);

            if(kill_length > max_kill_length){
                // If the newly-obtained kill is the longest, remove all former ones
                for(int i = 0; i < listGetLength(allowed_kills); i++){
                    TreeNode* n = listGet(allowed_kills, 0);
                    treeDestroy(n, 1);
                    listRemove(allowed_kills, 0, 0);
                }
                listAdd(allowed_kills, kills_from_pos);
                max_kill_length = kill_length;
            }else if(kill_length == max_kill_length){
                // If the length is equal to previous ones, just append it
                listAdd(allowed_kills, kills_from_pos);
            }else{
                // If it's too short, discard it
                treeDestroy(kills_from_pos, 1);
            }
        }
    }
    return allowed_kills;
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

/**
 * Checks all possible non-killing moves and returns a list of these which are allowed. 
 * The returned structure is a list of TreeNode's.
 * @param color The player's color whose move it is
 */
List* getAllowedNonKillingMoves(PawnColor color){
    List* allowed_moves = listCreate();
    int max_kill_length = 1;

    for(int row = 0; row < getBoardSize(); row++){
        for(int col = 0; col < getBoardSize(); col++){
            // Check if the source pawn is valid
            Pawn* p = getPawnAt(row, col);
            if(p == NULL) continue;
            if(getPawnColor(p) != color) continue;

            // Get legal moves from the source positions
            TreeNode* moves_from_pos = getAllowedMovesFrom(p, row, col);
            if(treeGetDepth(moves_from_pos) > 0) listAdd(allowed_moves, moves_from_pos);
            else treeDestroy(moves_from_pos, 1);
        }
    }
    return allowed_moves;
}

/**
 * Returns all allowed non-killing moves from the given field
 * @param p The pawn being moved
 * @param row The source row
 * @param col The source column
 */
TreeNode* getAllowedMovesFrom(Pawn* p, int row, int col){
    int rdir[] = {1, 1, -1, -1};
    int cdir[] = {1, -1, 1, -1};

    TreeNode* moves_tree = treeCreate(positionCreate(row, col, p));
    int max_depth = 0;

    for(int i = 0; i < 4; i++){
        int longest_move = isPawnKing(p) ? (getBoardSize() - 1) : 1;
        for(int j = 1; j <= longest_move; j++){
            int rto = row + j*rdir[i];
            int cto = col + j*cdir[i];
            if(checkMove(row, col, rto, cto) == MOVE_LEGAL){
                // Check if it's a non-killing move
                Position* killed_pos = getPawnAlongMove(row, col, rto, cto);
                Pawn* killed_pawn = positionGetPawn(killed_pos);
                positionDestroy(killed_pos);
                if(killed_pawn != NULL) break;

                TreeNode* target_subtree = treeCreate(positionCreate(rto, cto, NULL));
                treeAddChildNode(moves_tree, target_subtree);
            }
        }
    }

    return moves_tree;
}