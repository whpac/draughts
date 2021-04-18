#include<stddef.h>
#include "message.h"

char* messageTitle;
char* messageContent;

/**
 * Displays a message on screen
 * @param title The message title
 * @param content The message content
 */
void displayMessage(char* title, char* content){
    messageTitle = title;
    messageContent = content;
}

/** Hides the currently displayed message */
void hideMessage(){
    messageTitle = NULL;
    messageContent = NULL;
}

/** Checks whether there is a message to show */
char isMessageShown(){
    return (messageTitle != NULL) && (messageContent != NULL);
}

/** Returns the message title */
char* getMessageTitle(){
    return messageTitle;
}

/** Returns the message content */
char* getMessageContent(){
    return messageContent;
}