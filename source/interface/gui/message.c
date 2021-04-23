#include<stddef.h>
#include "../../data/stack.h"
#include "message.h"

Stack* titles = NULL;
Stack* contents = NULL;

/** Initializes the message controller */
void messageInit(){
    titles = stackCreate();
    contents = stackCreate();
}

/** Deinitializes the message controller */
void messageDeinit(){
    stackDestroy(titles);
    stackDestroy(contents);

    titles = contents = NULL;
}

/**
 * Displays a message on screen
 * @param title The message title
 * @param content The message content
 */
void displayMessage(char* title, char* content){
    if(titles == NULL || contents == NULL) return;

    stackPush(titles, title);
    stackPush(contents, content);
}

/** Hides the currently displayed message */
void hideMessage(){
    if(titles == NULL || contents == NULL) return;

    stackPop(titles);
    stackPop(contents);
}

/** Checks whether there is a message to show */
char isMessageShown(){
    return !stackIsEmpty(titles) && !stackIsEmpty(contents);
}

/** Returns the message title */
char* getMessageTitle(){
    return stackPeek(titles);
}

/** Returns the message content */
char* getMessageContent(){
    return stackPeek(contents);
}