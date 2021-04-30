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
#include "message.h"

#define FIELD_WIDTH 30.0
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
#define FRAME_CURSOR_THICKNESS 2.0
#define POINT_CURSOR_RADIUS 3.0
#define SHADOW_TEXT al_map_rgb(32, 0, 0)
#define DARK_GRAY_TEXT al_map_rgb(96, 96, 96)
#define DARK_WHITE_TEXT al_map_rgb(192, 192, 192)
#define LIGHT_WHITE_TEXT al_map_rgb(255, 255, 255)
#define MESSAGE_PLACEHOLDER_TEXT al_map_rgb(228, 160, 160)
#define MESSAGE_BACKGROUND al_map_rgb(96, 0, 0)
#define CHAR_WIDTH 8.0
#define SHADOW_OFFSET 2.0

void paintField(int row, int col);
void paintPawn(Pawn* p, int row, int col);
void paintMarker(int row, int col, MarkerColor marker_color, MarkerStyle marker_style);
void paintStatus();
void paintMessage();

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

    if(!isGameOver()){
        List* markers = guiGetMarkers();
        while(listGetLength(markers) > 0){
            Marker* marker = listGet(markers, 0);

            int crow = marker->row;
            int ccol = marker->col;
            paintMarker(crow, ccol, marker->color, marker->style);

            markerDestroy(marker);
            listRemove(markers, 0, 0);
        }
        listDestroy(markers, 0);
    }

    paintStatus();
    paintMessage();
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
 * Paints the marker on a given position.
 * @param row The row to point
 * @param col The column to point
 * @param marker_color The color of the marker
 * @param marker_style The style of the marker
 */
void paintMarker(int row, int col, MarkerColor marker_color, MarkerStyle marker_style){
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
            FIELD_WIDTH * col + 0.5 * FRAME_CURSOR_THICKNESS, FIELD_WIDTH * row + 0.5 * FRAME_CURSOR_THICKNESS,
            FIELD_WIDTH * (col + 1) - 0.5 * FRAME_CURSOR_THICKNESS, FIELD_WIDTH * (row + 1) - 0.5 * FRAME_CURSOR_THICKNESS,
            color, FRAME_CURSOR_THICKNESS
        );
    }else if(marker_style == point){
        al_draw_filled_circle(
            FIELD_WIDTH * (col + 0.5), FIELD_WIDTH * (row + 0.5),
            POINT_CURSOR_RADIUS, color
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
    char over = isGameOver();
    char msg_shown = isMessageShown();
    char str_buffer[3];

    int white_pawns = countPawnsOfColor(white);
    int black_pawns = countPawnsOfColor(black);

    // Print white's stats
    al_draw_text(font, DARK_WHITE_TEXT, 0.5 * CHAR_WIDTH, status_bar_y, ALLEGRO_ALIGN_LEFT, "WHITE");
    sprintf(str_buffer, "%02d", white_pawns % 100);
    al_draw_text(font, DARK_WHITE_TEXT, 6.5 * CHAR_WIDTH, status_bar_y, ALLEGRO_ALIGN_LEFT, str_buffer);

    // Print black's stats
    al_draw_text(font, DARK_WHITE_TEXT, window_width - 0.5 * CHAR_WIDTH, status_bar_y, ALLEGRO_ALIGN_RIGHT, "BLACK");
    sprintf(str_buffer, "%02d", black_pawns % 100);
    al_draw_text(font, DARK_WHITE_TEXT, window_width - 6.5 * CHAR_WIDTH, status_bar_y, ALLEGRO_ALIGN_RIGHT, str_buffer);

    char* move_indicator = "<MOVE ";
    if(getNextMoveColor() == black) move_indicator = " MOVE>";

    if(!over) al_draw_text(font, DARK_WHITE_TEXT, window_width / 2, status_bar_y, ALLEGRO_ALIGN_CENTER, move_indicator);

    char* arrows = "ARROWS: move";
    char* esc = msg_shown ? "ESC: close" : "ESC: exit";
    char* enter = "ENTER: select";
    char* undo = "U: undo";
    al_draw_text(font, (over || msg_shown) ? DARK_GRAY_TEXT : DARK_WHITE_TEXT, 0.5 * CHAR_WIDTH, status_bar_y + CHAR_WIDTH + 4, ALLEGRO_ALIGN_LEFT, arrows);
    al_draw_text(font, DARK_WHITE_TEXT, window_width - 0.5 * CHAR_WIDTH, status_bar_y + CHAR_WIDTH + 4, ALLEGRO_ALIGN_RIGHT, esc);
    al_draw_text(font, (over || msg_shown) ? DARK_GRAY_TEXT : DARK_WHITE_TEXT, 0.5 * CHAR_WIDTH, status_bar_y + 2*CHAR_WIDTH + 8, ALLEGRO_ALIGN_LEFT, enter);
    al_draw_text(font, msg_shown ? DARK_GRAY_TEXT : DARK_WHITE_TEXT, window_width - 0.5 * CHAR_WIDTH, status_bar_y + 2*CHAR_WIDTH + 8, ALLEGRO_ALIGN_RIGHT, undo);
}

/**
 * Displays the message box
 */
void paintMessage(){
    if(!isMessageShown()) return;

    ALLEGRO_FONT* font = guiGetFont();
    float board_length = getBoardSize() * FIELD_WIDTH;
    float stripe_height = 6 * CHAR_WIDTH;
    al_draw_filled_rectangle(
        0, (board_length - stripe_height) / 2,
        board_length, (board_length + stripe_height) / 2,
        MESSAGE_BACKGROUND
    );

    if(getMessageFlags() & MESSAGE_TITLE_SHADOW){
        al_draw_text(
            font, SHADOW_TEXT,
            board_length / 2 + SHADOW_OFFSET, (board_length - stripe_height) / 2 + CHAR_WIDTH + SHADOW_OFFSET,
            ALLEGRO_ALIGN_CENTER, getMessageTitle()
        );
    }
    al_draw_text(
        font, LIGHT_WHITE_TEXT,
        board_length / 2, (board_length - stripe_height) / 2 + CHAR_WIDTH,
        ALLEGRO_ALIGN_CENTER, getMessageTitle()
    );

    al_draw_text(
        font, isPlaceholderVisible() ? MESSAGE_PLACEHOLDER_TEXT : LIGHT_WHITE_TEXT,
        board_length / 2, (board_length - stripe_height) / 2 + CHAR_WIDTH * 3,
        ALLEGRO_ALIGN_CENTER, getMessageContent()
    );
}