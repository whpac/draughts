#ifndef MESSAGE_H
#define MESSAGE_H

void messageInit();
void messageDeinit();

void displayMessage(char* title, char* content);
void hideMessage();
char isMessageShown();

char* getMessageTitle();
char* getMessageContent();

#endif