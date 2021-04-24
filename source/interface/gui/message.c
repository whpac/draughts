#include<stddef.h>
#include<malloc.h>
#include "../../data/stack.h"
#include "message.h"

typedef struct Message Message;
struct Message {
    char* title;
    char* content;
};

Message* messageCreate();
void messageDestroy(Message* msg);

Stack* messages = NULL;

/** Initializes the message controller */
void messageInit(){
    messages = stackCreate();
}

/** Deinitializes the message controller */
void messageDeinit(){
    stackDestroy(messages);
    messages = NULL;
}

/** Allocates a memory for a new message instance */
Message* messageCreate(){
    return malloc(sizeof(Message));
}

/**
 * Destroys the message
 * @param msg The message to destroy
 */
void messageDestroy(Message* msg){
    free(msg);
}

/**
 * Displays a message on screen
 * @param title The message title
 * @param content The message content
 */
void displayMessage(char* title, char* content){
    if(messages == NULL) return;

    Message* msg = messageCreate();
    msg->title = title;
    msg->content = content;

    stackPush(messages, msg);
}

/** Hides the currently displayed message */
void hideMessage(){
    if(messages == NULL) return;

    messageDestroy(stackPop(messages));
}

/** Checks whether there is a message to show */
char isMessageShown(){
    return !stackIsEmpty(messages);
}

/** Returns the message title */
char* getMessageTitle(){
    Message* m = stackPeek(messages);
    return m->title;
}

/** Returns the message content */
char* getMessageContent(){
    Message* m = stackPeek(messages);
    return m->content;
}