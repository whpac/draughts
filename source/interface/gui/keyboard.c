#include<allegro5/allegro5.h>
#include "../../log/logger.h"
#include "display.h"
#include "controller.h"
#include "keyboard.h"
#include "message.h"

void processKeyPress(int keycode);
char processKeyPressMessage(ALLEGRO_KEYBOARD_EVENT kbd);
void redirectKeyToMessage(ALLEGRO_KEYBOARD_EVENT kbd);

enum { game, exitPrompt, logSavePrompt, logLoadPrompt } gameStatus = game;

/** Begins the keyboard input loop */
void guiBeginInputLoop(){
    guiPaintBoard();

    ALLEGRO_EVENT event;
    al_start_timer(guiGetTimer());
    while(1)
    {
        al_wait_for_event(guiGetEventQueue(), &event);

        char done = 0;
        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                guiPaint();
                break;

            case ALLEGRO_EVENT_KEY_CHAR:
                if(isMessageShown()){
                    done = processKeyPressMessage(event.keyboard);
                }else{
                    processKeyPress(event.keyboard.keycode);
                }
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = 1;
                break;
        }

        if(done)
            break;
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

    if(keycode == ALLEGRO_KEY_ESCAPE){
        displayMessage("QUIT?", "Press ENTER to exit.", MESSAGE_NORMAL);
        gameStatus = exitPrompt;
    }
}

/**
 * Processes the pressed key when there is a message on screen. 
 * If user requests to exit, returns 1.
 * @param kbd The keyboard event
 */
char processKeyPressMessage(ALLEGRO_KEYBOARD_EVENT kbd){
    if(guiIsWelcomeShown()){
        if(kbd.keycode == ALLEGRO_KEY_N){
            guiHideWelcome();
        }
        if(kbd.keycode == ALLEGRO_KEY_L){
            guiHideWelcome();
            displayMessage("LOAD FILE", "Type file name here", MESSAGE_EDITABLE | MESSAGE_FILE_FILTER);
            gameStatus = logLoadPrompt;
        }
        return 0;
    }

    if(kbd.keycode == ALLEGRO_KEY_ESCAPE){
        hideMessage();
        gameStatus = game;

    }else if(kbd.keycode == ALLEGRO_KEY_ENTER && gameStatus == exitPrompt){
        hideMessage();
        gameStatus = logSavePrompt;
        displayMessage("SAVE GAME", "Leave empty to discard game", MESSAGE_EDITABLE | MESSAGE_FILE_FILTER);

    }else if(kbd.keycode == ALLEGRO_KEY_ENTER && gameStatus == logSavePrompt){
        if(!isPlaceholderVisible()) setLogFileName(getMessageContent());
        else setLogFileName("");

        hideMessage();
        return 1;

    }else if(kbd.keycode == ALLEGRO_KEY_ENTER && gameStatus == logLoadPrompt){
        if(!isPlaceholderVisible()) setLogFileName(getMessageContent());
        else setLogFileName("");

        readLog(&guiAttemptMoveFromTo, &guiAttemptUndo);
        hideMessage();

    }else{
        redirectKeyToMessage(kbd);
    }
    return 0;
}

/**
 * Redirects a keypress to the message dialog
 * @param kbd The keyboard event
 */
void redirectKeyToMessage(ALLEGRO_KEYBOARD_EVENT kbd){
    appendCharToUserText((char)kbd.unichar);
}