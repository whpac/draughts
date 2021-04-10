#include<stddef.h>
#include<stdio.h>
#include <allegro5/allegro_primitives.h>
#include "../../board/board.h"
#include "../../board/pawn.h"
#include "../../data/list.h"
#include "painter.h"
#include "controller.h"
#include "marker.h"
#include "display.h"

#define FIELD_WIDTH 60.0
#define WHITE_FIELD al_map_rgb(172, 172, 172)
#define BLACK_FIELD al_map_rgb(0, 102, 102)
#define WHITE_PAWN_FILL al_map_rgb(255, 255, 255)
#define WHITE_PAWN_BORDER al_map_rgb(192, 192, 192)
#define BLACK_PAWN_FILL al_map_rgb(0, 0, 0)
#define BLACK_PAWN_BORDER al_map_rgb(64, 64, 64)
#define CURSOR_GREEN_COLOR al_map_rgb(0, 255, 0)
#define CURSOR_RED_COLOR al_map_rgb(255, 0, 0)
#define CURSOR_GOLD_COLOR al_map_rgb(255, 204, 0)
#define CURSOR_GRAY_COLOR al_map_rgb(172, 172, 172)

void paintField(int row, int col);
void paintPawn(Pawn* p, int row, int col);
void paintCursor(Pawn* p, int row, int col, MarkerColor marker_color, MarkerStyle marker_style);
void paintStatus();

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
        paintCursor(buffer[crow * board_size + ccol], crow, ccol, marker->color, marker->style);

        markerDestroy(marker);
        listRemove(markers, 0, 0);
    }
    listDestroy(markers, 0);

    paintStatus();
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
 * @param marker_color The color of the marker
 * @param marker_style The style of the marker
 */
void paintCursor(Pawn* p, int row, int col, MarkerColor marker_color, MarkerStyle marker_style){
    ALLEGRO_COLOR color = CURSOR_GREEN_COLOR;

    switch(marker_color){
        case red:
            color = CURSOR_RED_COLOR;
            break;
        case green:
            color = CURSOR_GREEN_COLOR;
            break;
        case gold:
            color = CURSOR_GOLD_COLOR;
            break;
        case gray:
            color = CURSOR_GRAY_COLOR;
            break;
    }

    if(marker_style == frame){
        al_draw_rectangle(
            FIELD_WIDTH * col + 2.0, FIELD_WIDTH * row + 2.0,
            FIELD_WIDTH * (col + 1) - 2.0, FIELD_WIDTH * (row + 1) - 2.0,
            color, 4.0
        );
    }else if(marker_style == point){
        al_draw_filled_circle(
            FIELD_WIDTH * (col + 0.5), FIELD_WIDTH * (row + 0.5),
            3.0, color
        );
    }
}

/**
 * Prints the number of pawns still in game, the currently moving player and some other info.
 */
void paintStatus(){
    ALLEGRO_FONT* font = guiGetFont();
    float status_bar_y = getBoardSize() * FIELD_WIDTH + 4;
    float window_width = getBoardSize() * FIELD_WIDTH;
    float char_width = 8;
    char str_buffer[3];
    int white_pawns, black_pawns;

    white_pawns = countPawnsOfColor(white);
    black_pawns = countPawnsOfColor(black);

    // Print white's stats
    al_draw_text(font, al_map_rgb(192, 192, 192), char_width, status_bar_y, ALLEGRO_ALIGN_LEFT, "WHITE");
    sprintf(str_buffer, "%02d", white_pawns % 100);
    al_draw_text(font, al_map_rgb(192, 192, 192), 8 * char_width, status_bar_y, ALLEGRO_ALIGN_LEFT, str_buffer);

    // Print black's stats
    al_draw_text(font, al_map_rgb(192, 192, 192), window_width - char_width, status_bar_y, ALLEGRO_ALIGN_RIGHT, "BLACK");
    sprintf(str_buffer, "%02d", black_pawns % 100);
    al_draw_text(font, al_map_rgb(192, 192, 192), window_width - 8 * char_width, status_bar_y, ALLEGRO_ALIGN_RIGHT, str_buffer);

    char* move_indicator = "< MOVE  ";
    if(getNextMoveColor() == black) move_indicator = "  MOVE >";
    if(white_pawns == 0) move_indicator = "GAME OVER! BLACK has won!";
    if(black_pawns == 0) move_indicator = "GAME OVER! WHITE has won!";

    al_draw_text(font, al_map_rgb(192, 192, 192), window_width / 2, status_bar_y, ALLEGRO_ALIGN_CENTER, move_indicator);

    char* arrows = "Use ARROW keys to move";
    char* esc = "Press ESC to exit";
    char* enter = "ENTER selects a pawn to move and confirms the target field";
    al_draw_text(font, al_map_rgb(192, 192, 192), char_width, status_bar_y + char_width + 4, ALLEGRO_ALIGN_LEFT, arrows);
    al_draw_text(font, al_map_rgb(192, 192, 192), window_width - char_width, status_bar_y + char_width + 4, ALLEGRO_ALIGN_RIGHT, esc);
    al_draw_text(font, al_map_rgb(192, 192, 192), char_width, status_bar_y + 2*char_width + 8, ALLEGRO_ALIGN_LEFT, enter);
}