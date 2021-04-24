#ifndef MESSAGE_H
#define MESSAGE_H

void messageInit();
void messageDeinit();

void displayMessage(char* title, char* content, char is_editable);
void hideMessage();
char isMessageShown();

char* getMessageTitle();
char* getMessageContent();

char isPlaceholderVisible();
void appendCharToUserText(char c);

#endif