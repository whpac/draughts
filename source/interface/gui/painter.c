#include <allegro5/allegro_primitives.h>
#include "painter.h"

#define FIELD_WIDTH 60.0
#define WHITE_FIELD al_map_rgb(48, 48, 48)
#define BLACK_FIELD al_map_rgb(172, 172, 172)

void paintField(int row, int col);
void paintPawn(Pawn* p, int row, int col);

float getFieldWidth(){
    return FIELD_WIDTH;
}

/**
 * Paints the game board
 * @param buffer An array of pointers to pawns
 * @param board_size The length of board side
 */
void paintBoard(Pawn** buffer, int board_size){
    for(int row = 0; row < board_size; row++){
        for(int col = 0; col < board_size; col++){
            paintField(row, col);

            Pawn* p = buffer[row * board_size + col];
            paintPawn(p, row, col);
        }
    }
}

/**
 * Paints an empty game field
 * @param row The row coordinate
 * @param col The column coordinate
 */
void paintField(int row, int col){
    ALLEGRO_COLOR color;
    if((row + col) % 2 == 1){
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
        fill_color = al_map_rgb(255, 255, 255);
        border_color = al_map_rgb(192, 192, 192);
    }else{
        fill_color = al_map_rgb(0, 0, 0);
        border_color = al_map_rgb(64, 64, 64);
    }

    al_draw_filled_circle(
        FIELD_WIDTH * (col + 0.5), FIELD_WIDTH * (row + 0.5),
        FIELD_WIDTH * 0.35, fill_color
    );
    al_draw_circle(
        FIELD_WIDTH * (col + 0.5), FIELD_WIDTH * (row + 0.5),
        FIELD_WIDTH * 0.35, border_color, 2.0
    );
}