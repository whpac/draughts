#include<allegro5/allegro5.h>
#include "display.h"
#include "controller.h"
#include "keyboard.h"

void guiBeginInputLoop(){
    guiPaintBoard();

    ALLEGRO_EVENT event;
    al_start_timer(guiGetTimer());
    while(1)
    {
        al_wait_for_event(guiGetEventQueue(), &event);

        char done = 0;
        char redraw = 0;
        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                redraw = 1;
                break;

            case ALLEGRO_EVENT_KEY_CHAR:
                if(event.keyboard.keycode == ALLEGRO_KEY_UP)
                    guiMoveCursor(-1, 0);
                if(event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                    guiMoveCursor(1, 0);
                if(event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                    guiMoveCursor(0, -1);
                if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                    guiMoveCursor(0, 1);

                if(event.keyboard.keycode == ALLEGRO_KEY_ENTER){
                    if(guiIsFieldSelected() && !guiIsCurrentFieldSelected()){
                        guiAttemptMoveFromSelectedToCursor();
                    }else{
                        guiSelectCurrentField(0);
                    }
                }

                if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    done = true;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = 1;
                break;
        }

        if(done)
            break;

        if(redraw)
            guiPaint();
    }
}