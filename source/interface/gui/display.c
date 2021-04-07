#include<stddef.h>
#include<malloc.h>
#include<allegro5/allegro5.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_primitives.h>
#include "display.h"
#include "painter.h"
#include "../../board/board.h"
#include "../../board/pawn.h"

ALLEGRO_TIMER* timer;
ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_DISPLAY* disp;
ALLEGRO_FONT* font;

Pawn** boardBuffer;

void guiCopyBoardToBuffer();

/** Initializes the GUI. Returns 0 on failure. */
char guiInit(){
    boardBuffer = malloc(sizeof(Pawn*) * getBoardSize() * getBoardSize());
    if(boardBuffer == NULL){
        return 0;
    }

    if(!al_init()) return 0;
    if(!al_install_keyboard()) return 0;
    if(!al_init_primitives_addon()) return 0;

    timer = al_create_timer(1.0 / 30.0);
    queue = al_create_event_queue();

    al_set_new_window_title("Draughts");
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    disp = al_create_display(getBoardSize() * getFieldWidth(), getBoardSize() * getFieldWidth());
    font = al_create_builtin_font();

    if(!timer || !queue || !disp || !font){
        al_destroy_font(font);
        al_destroy_display(disp);
        al_destroy_timer(timer);
        al_destroy_event_queue(queue);
        return 0;
    }

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    return 1;
}

/** Destroys the GUI */
void guiDestroy(){
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    free(boardBuffer);
}

/** Copies the board to the internal buffer. This prevents blinking when game is predicting moves */
void guiCopyBoardToBuffer(){
    int size = getBoardSize();
    for(int row = 0; row < size; row++){
        for(int col = 0; col < size; col++){
            boardBuffer[row * size + col] = getPawnAt(row, col);
        }
    }
}

/** Forces the program to read the current board state and repaint it */
void guiDisplayBoard(){
    guiCopyBoardToBuffer();
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "DRAUGHTS");
    paintBoard(boardBuffer, getBoardSize());
    al_flip_display();

    ALLEGRO_EVENT event;
    al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);

        char done = 0;
        switch(event.type)
        {
            case ALLEGRO_EVENT_KEY_DOWN:
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = 1;
                break;
        }

        if(done)
            break;
    }
}