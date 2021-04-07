#include<stddef.h>
#include<malloc.h>
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

/** Returns the GUI timer */
ALLEGRO_TIMER* guiGetTimer(){
    return timer;
}

/** Returns the GUI event queue */
ALLEGRO_EVENT_QUEUE* guiGetEventQueue(){
    return queue;
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

/** Forces the program to read the current board state and repaint it */
void guiPaintBoard(){
    paintBoard(boardBuffer, getBoardSize());
    al_flip_display();
}