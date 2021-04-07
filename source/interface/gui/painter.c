#include<stddef.h>
#include <allegro5/allegro_primitives.h>
#include "../../board/board.h"
#include "../../board/pawn.h"
#include "../../data/list.h"
#include "painter.h"
#include "controller.h"
#include "marker.h"

#define FIELD_WIDTH 60.0
#define WHITE_FIELD al_map_rgb(172, 172, 172)
#define BLACK_FIELD al_map_rgb(0, 102, 102)
#define WHITE_PAWN_FILL al_map_rgb(255, 255, 255)
#define WHITE_PAWN_BORDER al_map_rgb(192, 192, 192)
#define BLACK_PAWN_FILL al_map_rgb(0, 0, 0)
#define BLACK_PAWN_BORDER al_map_rgb(64, 64, 64)
#define CURSOR_OK_COLOR al_map_rgb(0, 255, 0)
#define CURSOR_ERR_COLOR al_map_rgb(255, 0, 0)

void paintField(int row, int col);
void paintPawn(Pawn* p, int row, int col);
void paintCursor(Pawn* p, int row, int col);

float getFieldWidth(){
    return FIELD_WIDTH;
}

/**
 * Paints the game board
 * @param buffer An array of pointers to pawns
 * @param board_size The length of board side
 */
void paintBoard(Pawn** buffer, int board_size){
    al_clear_to_color(al_map_rgb(0, 0, 0));

    for(int row = 0; row < board_size; row++){
        for(int col = 0; col < board_size; col++){
            paintField(row, col);

            Pawn* p = buffer[row * board_size + col];
            paintPawn(p, row, col);
        }
    }

    List* markers = guiGetMarkers();
    while(listGetLength(markers) > 0){
        Marker* marker = listGet(markers, 0);

        int crow = marker->row;
        int ccol = marker->col;
        paintCursor(buffer[crow * board_size + ccol], crow, ccol);

        markerDestroy(marker);
        listRemove(markers, 0, 0);
    }
    listDestroy(markers, 0);
}

/**
 * Paints an empty game field
 * @param row The row coordinate
 * @param col The column coordinate
 */
void paintField(int row, int col){
    ALLEGRO_COLOR color;
    if((row + col) % 2 == 0){
        color = WHITE_FIELD;
    }else{
        color = BLACK_FIELD;
    }

    al_draw_filled_rectangle(
        FIELD_WIDTH * col, FIELD_WIDTH * row,
        FIELD_WIDTH * (col + 1), FIELD_WIDTH * (row + 1),
        color
    );
}

/**
 * Paints a pawn on the specified field
 * @param p The pawn to be painted
 * @param row The row
 * @param col The column
 */
void paintPawn(Pawn* p, int row, int col){
    if(p == NULL) return;

    ALLEGRO_COLOR fill_color, border_color;
    if(getPawnColor(p) == white){
        fill_color = WHITE_PAWN_FILL;
        border_color = WHITE_PAWN_BORDER;
    }else{
        fill_color = BLACK_PAWN_FILL;
        border_color = BLACK_PAWN_BORDER;
    }

    al_draw_filled_circle(
        FIELD_WIDTH * (col + 0.5), FIELD_WIDTH * (row + 0.5),
        FIELD_WIDTH * 0.35, fill_color
    );
    al_draw_circle(
        FIELD_WIDTH * (col + 0.5), FIELD_WIDTH * (row + 0.5),
        FIELD_WIDTH * 0.35, border_color, 2.0
    );

    if(isPawnKing(p)){
        al_draw_filled_circle(
            FIELD_WIDTH * (col + 0.5), FIELD_WIDTH * (row + 0.5),
            FIELD_WIDTH * 0.2, border_color
        );
    }
}

/**
 * Paints the cursor on a given position.
 * @param p The pawn pointed at by the cursor
 * @param row The row to point
 * @param col The column to point
 */
void paintCursor(Pawn* p, int row, int col){
    ALLEGRO_COLOR color = CURSOR_OK_COLOR;

    if(!isPlayableField(row, col)) color = CURSOR_ERR_COLOR;
    if(p == NULL) color = CURSOR_ERR_COLOR;
    else if(getPawnColor(p) != getNextMoveColor()) color = CURSOR_ERR_COLOR;

    al_draw_rectangle(
        FIELD_WIDTH * col + 2.0, FIELD_WIDTH * row + 2.0,
        FIELD_WIDTH * (col + 1) - 2.0, FIELD_WIDTH * (row + 1) - 2.0,
        color, 4.0
    );
}