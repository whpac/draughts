#include<allegro5/allegro5.h>
#include "../../log/logger.h"
#include "display.h"
#include "controller.h"
#include "keyboard.h"
#include "message.h"

void processKeyPress(int keycode);
void redirectKeyToMessage(ALLEGRO_KEYBOARD_EVENT kbd);

/** Begins the keyboard input loop */
void guiBeginInputLoop(){
    guiPaintBoard();

    ALLEGRO_EVENT event;
    enum { no_exit, prompt, file_prompt } exit_status = no_exit;
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
                if(isMessageShown()){
                    if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                        hideMessage();
                        exit_status = no_exit;
                    }else if(event.keyboard.keycode == ALLEGRO_KEY_ENTER && exit_status == prompt){
                        hideMessage();
                        exit_status = file_prompt;
                        displayMessage("SAVE LOG", "Leave empty to discard log", editable_file_filter);
                    }else if(event.keyboard.keycode == ALLEGRO_KEY_ENTER && exit_status == file_prompt){
                        if(!isPlaceholderVisible()) setLogFileName(getMessageContent());
                        else setLogFileName("");

                        hideMessage();
                        done = 1;
                    }else{
                        redirectKeyToMessage(event.keyboard);
                    }
                }else{
                    processKeyPress(event.keyboard.keycode);

                    if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                        displayMessage("QUIT?", "Press ENTER to exit.", non_editable);
                        exit_status = prompt;
                    }
                }
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

/** Processes the pressed key */
void processKeyPress(int keycode){
    if(keycode == ALLEGRO_KEY_UP)
        guiMoveCursor(-1, 0);
    if(keycode == ALLEGRO_KEY_DOWN)
        guiMoveCursor(1, 0);
    if(keycode == ALLEGRO_KEY_LEFT)
        guiMoveCursor(0, -1);
    if(keycode == ALLEGRO_KEY_RIGHT)
        guiMoveCursor(0, 1);

    if(keycode == ALLEGRO_KEY_ENTER){
        if(guiIsFieldSelected() && !guiIsCurrentFieldSelected()){
            guiAttemptMoveFromSelectedToCursor();
        }else{
            guiSelectCurrentField(0);
        }
    }

    if(keycode == ALLEGRO_KEY_U)
        guiAttemptUndo();
}

/**
 * Redirects a keypress to the message dialog
 * @param kbd The keyboard event
 */
void redirectKeyToMessage(ALLEGRO_KEYBOARD_EVENT kbd){
    appendCharToUserText((char)kbd.unichar);
}