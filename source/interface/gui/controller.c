#include<malloc.h>
#include<allegro5/allegro5.h>
#include "../../board/board.h"
#include "controller.h"
#include "marker.h"
#include "painter.h"

int cursorRow = 0, cursorCol = 0;
Pawn** boardBuffer;

MarkerColor getCursorColor();

/** Initializes the GUI controller */
void guiInitController(){
    boardBuffer = malloc(sizeof(Pawn*) * getBoardSize() * getBoardSize());
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

/** Returns a list of game board markers */
List* guiGetMarkers(){
    List* markers = listCreate();

    Marker* m = markerCreate();
    m->row = cursorRow;
    m->col = cursorCol;
    m->color = getCursorColor();
    listAdd(markers, m);

    return markers;
}

/** Returns a color for the cursor */
MarkerColor getCursorColor(){
    MarkerColor color = green;
    Pawn* p = boardBuffer[cursorRow * getBoardSize() + cursorCol];

    if(!isPlayableField(cursorRow, cursorCol)) color = red;
    if(p == NULL) color = red;
    else if(getPawnColor(p) != getNextMoveColor()) color = red;

    return color;
}