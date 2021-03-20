#ifndef CMD_PROCESSOR_H
#define CMD_PROCESSOR_H

char getHelpChar();
char getQuitChar();

char processCommand(char cmd, int (*int_getter)());

#endif