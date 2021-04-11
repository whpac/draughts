#include<stddef.h>
#include<malloc.h>
#include<allegro5/allegro_primitives.h>
#include "display.h"
#include "painter.h"
#include "controller.h"
#include "../../board/board.h"
#include "../../board/pawn.h"

#define DISPLAY_SCALE 2.0

ALLEGRO_TIMER* timer;
ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_DISPLAY* disp;
ALLEGRO_BITMAP* buffer;
ALLEGRO_FONT* font;

int buffer_w, buffer_h;

/** Initializes the GUI. Returns 0 on failure. */
char guiInit(){
    guiInitController();

    if(!al_init()) return 0;
    if(!al_install_keyboard()) return 0;
    if(!al_init_primitives_addon()) return 0;

    timer = al_create_timer(1.0 / 30.0);
    queue = al_create_event_queue();

    al_set_new_window_title("Draughts");

    buffer_w = getBoardSize() * getFieldWidth();
    buffer_h = getBoardSize() * getFieldWidth() + 40;
    disp = al_create_display(buffer_w * DISPLAY_SCALE, buffer_h * DISPLAY_SCALE);
    buffer = al_create_bitmap(buffer_w, buffer_h);

    font = al_create_builtin_font();

    if(!timer || !queue || !disp || !buffer || !font){
        guiDestroy();
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
    al_destroy_bitmap(buffer);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    guiDeinitController();
}

/** Orders the game to paint the display */
void guiPaint(){
    al_set_target_bitmap(buffer);
    guiPaintBoard();

    al_set_target_backbuffer(disp);
    al_draw_scaled_bitmap(buffer, 0, 0, buffer_w, buffer_h, 0, 0, buffer_w * DISPLAY_SCALE, buffer_h * DISPLAY_SCALE, 0);
    al_flip_display();
}

/** Returns the GUI timer */
ALLEGRO_TIMER* guiGetTimer(){
    return timer;
}

/** Returns the GUI event queue */
ALLEGRO_EVENT_QUEUE* guiGetEventQueue(){
    return queue;
}

/** Returns the font */
ALLEGRO_FONT* guiGetFont(){
    return font;
}
