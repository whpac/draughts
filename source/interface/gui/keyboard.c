#include<allegro5/allegro5.h>
#include "../../log/logger.h"
#include "display.h"
#include "controller.h"
#include "keyboard.h"
#include "message.h"

/**
 * General signature for key processors. 
 * Should return 0 to continue or 1 to exit the game.
 * @param kbd The keyboard event data
 */
typedef int (*KeyInterpreter)(ALLEGRO_KEYBOARD_EVENT kbd);

int keyPressedGame(ALLEGRO_KEYBOARD_EVENT kbd);
int keyPressedGameOver(ALLEGRO_KEYBOARD_EVENT kbd);
int keyPressedWelcome(ALLEGRO_KEYBOARD_EVENT kbd);
int keyPressedLogLoadPrompt(ALLEGRO_KEYBOARD_EVENT kbd);
int keyPressedLogLoadError(ALLEGRO_KEYBOARD_EVENT kbd);
int keyPressedExitPrompt(ALLEGRO_KEYBOARD_EVENT kbd);
int keyPressedLogSavePrompt(ALLEGRO_KEYBOARD_EVENT kbd);
int keyPressedLogSaveError(ALLEGRO_KEYBOARD_EVENT kbd);
int keyPressedMessage(ALLEGRO_KEYBOARD_EVENT kbd);

void displayExitPrompt();
void displayLogLoadPrompt();
void displayLogSavePrompt();

/** Begins the keyboard input loop */
void guiBeginInputLoop(){
    KeyInterpreter interpreters[8];
    interpreters[welcome] = keyPressedWelcome;
    interpreters[logLoadPrompt] = keyPressedLogLoadPrompt;
    interpreters[logLoadError] = keyPressedLogLoadError;
    interpreters[game] = keyPressedGame;
    interpreters[gameOver] = keyPressedGameOver;
    interpreters[exitPrompt] = keyPressedExitPrompt;
    interpreters[logSavePrompt] = keyPressedLogSavePrompt;
    interpreters[logSaveError] = keyPressedLogSaveError;

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

            case ALLEGRO_EVENT_KEY_CHAR:;
                KeyInterpreter interpret = interpreters[guiGetGameStatus()];
                done = interpret(event.keyboard);
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:;
                GameStatus gs = guiGetGameStatus();
                if(gs == game || gs == gameOver){
                    displayExitPrompt();
                }else{
                    done = 1;
                }
                break;
        }

        if(done)
            break;
    }
}

/** Processes the pressed key during the game */
int keyPressedGame(ALLEGRO_KEYBOARD_EVENT kbd){
    if(isMessageShown()) return keyPressedMessage(kbd);

    int keycode = kbd.keycode;
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

    if(keycode == ALLEGRO_KEY_ESCAPE)
        displayExitPrompt();

    return 0;
}

/** Processes the pressed key when the game is over */
int keyPressedGameOver(ALLEGRO_KEYBOARD_EVENT kbd){
    if(kbd.keycode == ALLEGRO_KEY_U && !isMessageShown())
        guiAttemptUndo();

    if(kbd.keycode == ALLEGRO_KEY_ESCAPE){
        if(isMessageShown()) hideMessage();
        else{
            displayMessage("QUIT?", "Press ENTER to exit.", MESSAGE_NORMAL);
            guiSetGameStatus(exitPrompt);
        }
    }

    return 0;
}

/** Processes the pressed key when there is a welcome message */
int keyPressedWelcome(ALLEGRO_KEYBOARD_EVENT kbd){
    if(kbd.keycode == ALLEGRO_KEY_N){
        hideMessage();
        guiSetGameStatus(game);

    }else if(kbd.keycode == ALLEGRO_KEY_L){
        hideMessage();
        displayLogLoadPrompt();
    }
    return 0;
}

/** Processes the pressed key when there is a log load prompt */
int keyPressedLogLoadPrompt(ALLEGRO_KEYBOARD_EVENT kbd){
    if(kbd.keycode == ALLEGRO_KEY_ESCAPE){
        hideMessage();
        guiSetGameStatus(game);

    }else if(kbd.keycode == ALLEGRO_KEY_ENTER){
        if(!isPlaceholderVisible()) setLogFileName(getMessageContent());
        else setLogFileName("");

        hideMessage();
        int err = readLog(&guiAttemptMoveFromTo, &guiAttemptUndo);
        if(err){
            char* error_msg = "Cannot open file";
            if(err == LOGGER_FILE_INVALID) error_msg = "File is invalid";
            displayMessage("READ ERROR", error_msg, MESSAGE_NORMAL);
            guiSetGameStatus(logLoadError);
        }else{
            guiSetGameStatus(game);
        }
    }else{
        appendCharToUserText((char)kbd.unichar);
    }
    return 0;
}

/** Processes the pressed key when there is a log load error message */
int keyPressedLogLoadError(ALLEGRO_KEYBOARD_EVENT kbd){
    if(kbd.keycode == ALLEGRO_KEY_ESCAPE){
        hideMessage();
        guiSetGameStatus(game);
    }else{
        hideMessage();
        displayLogLoadPrompt();
    }
    return 0;
}

/** Processes the pressed key when there is an exit prompt */
int keyPressedExitPrompt(ALLEGRO_KEYBOARD_EVENT kbd){
    if(kbd.keycode == ALLEGRO_KEY_ESCAPE){
        hideMessage();
        guiSetGameStatus(game);

    }else if(kbd.keycode == ALLEGRO_KEY_ENTER){
        hideMessage();
        displayLogSavePrompt();
    }
    return 0;
}

/** Processes the pressed key when there is a log save prompt */
int keyPressedLogSavePrompt(ALLEGRO_KEYBOARD_EVENT kbd){
    if(kbd.keycode == ALLEGRO_KEY_ESCAPE){
        hideMessage();
        guiSetGameStatus(game);

    }else if(kbd.keycode == ALLEGRO_KEY_ENTER){
        if(!isPlaceholderVisible()) setLogFileName(getMessageContent());
        else setLogFileName("");

        hideMessage();
        if(!saveLog()){
            return 1;
        }

        displayMessage("ERROR", "Cannot open file", MESSAGE_NORMAL);
        guiSetGameStatus(logSaveError);
    }else{
        appendCharToUserText((char)kbd.unichar);
    }
    return 0;
}

/** Processes the pressed key when there is a log save error message */
int keyPressedLogSaveError(ALLEGRO_KEYBOARD_EVENT kbd){
    if(kbd.keycode == ALLEGRO_KEY_ESCAPE){
        hideMessage();
        return 1;
    }else{
        hideMessage();
        displayLogSavePrompt();
    }
    return 0;
}

/** Processes the pressed key when there is a message on screen */
int keyPressedMessage(ALLEGRO_KEYBOARD_EVENT kbd){
    if(kbd.keycode == ALLEGRO_KEY_ESCAPE){
        hideMessage();

    }else{
        appendCharToUserText((char)kbd.unichar);
    }
    return 0;
}

/** Displays an exit prompt */
void displayExitPrompt(){
    displayMessage("QUIT?", "Press ENTER to exit.", MESSAGE_NORMAL);
    guiSetGameStatus(exitPrompt);
}

/** Displays a log load prompt */
void displayLogLoadPrompt(){
    displayMessage("LOAD FILE", "Type file name here", MESSAGE_EDITABLE | MESSAGE_FILE_FILTER);
    guiSetGameStatus(logLoadPrompt);
}

/** Displays a log save prompt */
void displayLogSavePrompt(){
    displayMessage("SAVE GAME", "Leave empty to discard game", MESSAGE_EDITABLE | MESSAGE_FILE_FILTER);
    guiSetGameStatus(logSavePrompt);
}
