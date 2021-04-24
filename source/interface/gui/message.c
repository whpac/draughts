#include<stddef.h>
#include<malloc.h>
#include "../../data/stack.h"
#include "message.h"

#define USER_TEXT_LENGTH 16

typedef struct Message Message;
struct Message {
    char* title;
    char* content;
    char isEditable;
    char* userText;
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
    Message* m = malloc(sizeof(Message));
    m->title = m->content = "";
    m->isEditable = 0;
    m->userText = NULL;
    return m;
}

/**
 * Destroys the message
 * @param msg The message to destroy
 */
void messageDestroy(Message* msg){
    if(msg->userText != NULL) free(msg->userText);
    free(msg);
}

/**
 * Displays a message on screen
 * @param title The message title
 * @param content The message content
 * @param is_editable Whether the content is editable
 */
void displayMessage(char* title, char* content, char is_editable){
    if(messages == NULL) return;

    Message* msg = messageCreate();
    msg->title = title;
    msg->content = content;
    msg->isEditable = is_editable;

    if(is_editable){
        msg->userText = malloc(sizeof(char) * (USER_TEXT_LENGTH + 1));
        for(int i = 0; i <= USER_TEXT_LENGTH; i++) msg->userText[i] = '\0';
    }

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
    if(m->isEditable && m->userText[0] != '\0') return m->userText;
    return m->content;
}

/** Checks whether the placeholder is visible */
char isPlaceholderVisible(){
    Message* m = stackPeek(messages);
    if(!m->isEditable) return 0;
    if(m->userText[0] != '\0') return 0;
    return 1;
}

/**
 * Appends a character to the editable text
 * @param c A letter to append
 */
void appendCharToUserText(char c){
    Message* m = stackPeek(messages);
    for(int i = 0; i < USER_TEXT_LENGTH; i++){
        if(m->userText[i] == '\0' && c != '\b'){
            m->userText[i] = c;
            break;
        }
        if(m->userText[i+1] == '\0' && c == '\b'){
            m->userText[i] = '\0';
            break;
        }
    }
}