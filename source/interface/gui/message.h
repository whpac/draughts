#ifndef MESSAGE_H
#define MESSAGE_H

#define MESSAGE_NORMAL 0
#define MESSAGE_EDITABLE 1
#define MESSAGE_FILE_FILTER 2
#define MESSAGE_TITLE_SHADOW 4

void messageInit();
void messageDeinit();

void displayMessage(char* title, char* content, int flags);
void hideMessage();
char isMessageShown();

char* getMessageTitle();
char* getMessageContent();
int getMessageFlags();

char isPlaceholderVisible();
void appendCharToUserText(char c);

#endif