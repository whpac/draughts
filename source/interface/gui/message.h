#ifndef MESSAGE_H
#define MESSAGE_H

typedef enum MessageEditable MessageEditable;
enum MessageEditable { non_editable = 0, editable = 1, editable_file_filter = 2};

void messageInit();
void messageDeinit();

void displayMessage(char* title, char* content, MessageEditable is_editable);
void hideMessage();
char isMessageShown();

char* getMessageTitle();
char* getMessageContent();

char isPlaceholderVisible();
void appendCharToUserText(char c);

#endif