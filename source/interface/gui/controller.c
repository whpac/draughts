#include "../../board/board.h"
#include "controller.h"
#include "marker.h"

int cursorRow = 0, cursorCol = 0;

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

List* guiGetMarkers(){
    List* markers = listCreate();

    Marker* m = markerCreate();
    m->row = cursorRow;
    m->col = cursorCol;
    m->color = green;
    listAdd(markers, m);

    return markers;
}