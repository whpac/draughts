#include<malloc.h>
#include<allegro5/allegro5.h>
#include "../../board/board.h"
#include "controller.h"
#include "marker.h"
#include "painter.h"

int cursorRow = 0, cursorCol = 0;
int selectedRow = -1, selectedCol = -1;
char selectionFrozen = 0;
Pawn** boardBuffer;
enum { waitingForSource, waitingForDestination } currentState;

MarkerColor getCursorColor();
void guiDeselectField(char unfreeze);
char isValidSourceField(int row, int col);
char isValidDestinationField(int rfrom, int cfrom, int rto, int cto);

/** Initializes the GUI controller */
void guiInitController(){
    boardBuffer = malloc(sizeof(Pawn*) * getBoardSize() * getBoardSize());
    currentState = waitingForSource;
}

/** Deinitializes the controller */
void guiDeinitController(){
    free(boardBuffer);
}

/** Forces the program to read the current board state and repaint it */
void guiPaintBoard(){
    paintBoard(boardBuffer, getBoardSize());
    al_flip_display();
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

    m = markerCreate();
    m->row = cursorRow;
    m->col = cursorCol;
    m->color = getCursorColor();
    listAdd(markers, m);

    if(guiIsFieldSelected()){
        m = markerCreate();
        m->row = selectedRow;
        m->col = selectedCol;
        m->color = gold;
        listAdd(markers, m);
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
    Pawn* p = boardBuffer[cursorRow * getBoardSize() + cursorCol];

    if(!isPlayableField(cursorRow, cursorCol)) return 0;
    if(p == NULL) return 0;
    else if(getPawnColor(p) != getNextMoveColor()) return 0;

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
    return 1;
}