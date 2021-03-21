#include<stddef.h>
#include "kills.h"
#include "board.h"
#include "moves.h"

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