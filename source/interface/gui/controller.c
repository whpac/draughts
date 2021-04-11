#include<stddef.h>
#include<malloc.h>
#include<allegro5/allegro5.h>
#include "../../board/board.h"
#include "../../board/kills.h"
#include "../../board/position.h"
#include "../../data/tree.h"
#include "../../data/list.h"
#include "controller.h"
#include "marker.h"
#include "painter.h"

int cursorRow = 0, cursorCol = 0;
int selectedRow = -1, selectedCol = -1;
char selectionFrozen = 0;
Pawn** boardBuffer;
List* allowedMoves = NULL;
enum { waitingForSource, waitingForDestination } currentState;

void guiReloadBoard();
void guiLoadAllowedMovesCache(char only_from_cursor);
void guiDestroyAllowedMovesCache();
MarkerColor getCursorColor();
void guiDeselectField(char unfreeze);
char isValidSourceField(int row, int col);
char isValidDestinationField(int rfrom, int cfrom, int rto, int cto);

/** Initializes the GUI controller */
void guiInitController(){
    boardBuffer = malloc(sizeof(Pawn*) * getBoardSize() * getBoardSize());
    currentState = waitingForSource;

    guiReloadBoard();
    guiLoadAllowedMovesCache(0);
}

/** Deinitializes the controller */
void guiDeinitController(){
    free(boardBuffer);
    guiDestroyAllowedMovesCache();
}

/** Forces the program to repaint the board */
void guiPaintBoard(){
    paintBoard(boardBuffer, getBoardSize());
}

/** Copies the board to the internal buffer. This prevents blinking when game is predicting moves */
void guiReloadBoard(){
    int size = getBoardSize();
    for(int row = 0; row < size; row++){
        for(int col = 0; col < size; col++){
            boardBuffer[row * size + col] = getPawnAt(row, col);
        }
    }
}

/**
 * Loads the list of allowed moves
 * @param only_from_cursor Whether to check only those moves originating from cursor
 */
void guiLoadAllowedMovesCache(char only_from_cursor){
    guiDestroyAllowedMovesCache();
    if(!only_from_cursor){
        allowedMoves = getAllowedKills(getNextMoveColor());
    }else{
        allowedMoves = listCreate();
        listAdd(allowedMoves,
            getAllowedKillsFrom(getPawnAt(cursorRow, cursorCol), cursorRow, cursorCol));
    }

    if(listGetLength(allowedMoves) == 0){
        listDestroy(allowedMoves, 0);
        allowedMoves = getAllowedNonKillingMoves(getNextMoveColor());
    }
}

/** Destroys the list of allowed moves */
void guiDestroyAllowedMovesCache(){
    if(allowedMoves == NULL) return;

    int allowed_moves_length = listGetLength(allowedMoves);
    for(int i = 0; i < allowed_moves_length; i++){
        TreeNode* n = listGet(allowedMoves, 0);
        treeDestroy(n, 1);
        listRemove(allowedMoves, 0, 0);
    }
    listDestroy(allowedMoves, 0);
}

/**
 * Moves the cursor by a given number of fields in the specified directions
 * @param drow The difference in row number
 * @param dcol The difference in column number
 */
void guiMoveCursor(int drow, int dcol){
    cursorRow += drow;
    cursorCol += dcol;
    int size = getBoardSize();

    if(cursorRow < 0) cursorRow = 0;
    if(cursorCol < 0) cursorCol = 0;
    if(cursorRow >= size) cursorRow = size - 1;
    if(cursorCol >= size) cursorCol = size - 1;
}

/**
 * Selects the currently pointed field. 
 * If the pointed field is already selected, deselects it. 
 * If the pointed field is not a valid source field, it doesn't change selection.
 * @param freeze If set to 1, the current selection can be changed only after a successful move.
 */
void guiSelectCurrentField(char freeze){
    if(cursorRow == selectedRow && cursorCol == selectedCol){
        guiDeselectField(0);
    }else{
        if(!isValidSourceField(cursorRow, cursorCol)) return;
        selectedRow = cursorRow;
        selectedCol = cursorCol;
        currentState = waitingForDestination;
        selectionFrozen = selectionFrozen || freeze;
    }
}

/**
 * Deselects the currently selected field.
 * @param unfreeze If set to 1, deselects also the frozen field
 */
void guiDeselectField(char unfreeze){
    if(selectionFrozen && !unfreeze) return;

    selectedRow = selectedCol = -1;
    currentState = waitingForSource;
    selectionFrozen = 0;
}

/** Checks whether there is a selected field */
char guiIsFieldSelected(){
    return selectedRow >= 0 && selectedCol >= 0;
}

/** Checks whether the pointed field is selected */
char guiIsCurrentFieldSelected(){
    return selectedRow == cursorRow && selectedCol == cursorCol;
}

/** Returns a list of game board markers */
List* guiGetMarkers(){
    List* markers = listCreate();
    Marker* m;

    // A reference point for possible target fields
    int ref_row = cursorRow, ref_col = cursorCol;

    m = markerCreate();
    m->row = cursorRow;
    m->col = cursorCol;
    m->color = getCursorColor();
    m->style = frame;
    listAdd(markers, m);

    if(guiIsFieldSelected()){
        m = markerCreate();
        m->row = selectedRow;
        m->col = selectedCol;
        m->color = gold;
        m->style = frame;
        listAdd(markers, m);

        ref_row = selectedRow;
        ref_col = selectedCol;
    }

    int moves_count = listGetLength(allowedMoves);
    for(int i = 0; i < moves_count; i++){
        TreeNode* n = listGet(allowedMoves, i);
        Position* src_pos = treeGetNodeContent(n);

        if(positionGetRow(src_pos) == ref_row && positionGetColumn(src_pos) == ref_col){
            int target_count = treeGetChildNodesCount(n);
            for(int j = 0; j < target_count; j++){
                TreeNode* target = treeGetChildNode(n, j);
                Position* target_pos = treeGetNodeContent(target);

                m = markerCreate();
                m->row = positionGetRow(target_pos);
                m->col = positionGetColumn(target_pos);
                m->color = gray;
                m->style = point;
                listAdd(markers, m);
            }

            break;
        }
    }

    return markers;
}

/**
 * Attempts to move the selected pawn to the currently pointed field.
 */
int guiAttemptMoveFromSelectedToCursor(){
    int result = attemptMovePawnAtTo(selectedRow, selectedCol, cursorRow, cursorCol);

    if(result == BOARD_MOVE_SUCCESSFUL || result == BOARD_MOVE_NOT_FINISHED){
        guiDeselectField(1);
        guiReloadBoard();
        guiLoadAllowedMovesCache(result == BOARD_MOVE_NOT_FINISHED);
    }

    if(result == BOARD_MOVE_NOT_FINISHED){
        guiSelectCurrentField(1);
    }

    return result;
}

/** Returns a color for the cursor */
MarkerColor getCursorColor(){
    if(currentState == waitingForSource){
        return isValidSourceField(cursorRow, cursorCol) ? green : red;
    }else{
        return isValidDestinationField(selectedRow, selectedCol, cursorRow, cursorCol) ? green : red;
    }
}

/**
 * Checks whether the specified field is a valid source field
 * @param row The row coordinate
 * @param col The column coordinate
 */
char isValidSourceField(int row, int col){
    Pawn* p = boardBuffer[row * getBoardSize() + col];

    if(!isPlayableField(row, col)) return 0;
    if(p == NULL) return 0;
    else if(getPawnColor(p) != getNextMoveColor()) return 0;

    int allowed_moves_length = listGetLength(allowedMoves);

    // Check for the requested source field
    char is_valid = 0;
    for(int i = 0; i < allowed_moves_length; i++){
        TreeNode* n = listGet(allowedMoves, i);
        Position* pos = treeGetNodeContent(n);

        if(positionGetRow(pos) == row && positionGetColumn(pos) == col){
            is_valid = 1;
            break;
        }
    }

    if(!is_valid) return 0;

    return 1;
}

/**
 * Checks whether the (rfrom, cfrom) -> (rto, cto) pair forms a valid move.
 * @param rfrom The source row
 * @param cfrom The source column
 * @param rto The destination row
 * @param cto The destination column
 */
char isValidDestinationField(int rfrom, int cfrom, int rto, int cto){
    Pawn* p = boardBuffer[rto * getBoardSize() + cto];

    if(!isPlayableField(rto, cto)) return 0;
    if(p != NULL) return 0;

    int allowed_moves_length = listGetLength(allowedMoves);

    char is_valid = 0;
    // Look for the requested source field
    for(int i = 0; i < allowed_moves_length && !is_valid; i++){
        TreeNode* n = listGet(allowedMoves, i);
        Position* pos = treeGetNodeContent(n);

        if(positionGetRow(pos) == rfrom && positionGetColumn(pos) == cfrom){
            int tree_nodes_cnt = treeGetChildNodesCount(n);
            // Look for the destination field
            for(int j = 0; j < tree_nodes_cnt; j++){
                TreeNode* target = treeGetChildNode(n, j);
                Position* target_pos = treeGetNodeContent(target);

                if(positionGetRow(target_pos) == rto && positionGetColumn(target_pos) == cto){
                    is_valid = 1;
                    break;
                }
            }
        }
    }

    if(!is_valid) return 0;

    return 1;
}