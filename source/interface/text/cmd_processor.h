#ifndef CMD_PROCESSOR_H
#define CMD_PROCESSOR_H

#define CMD_PROC_SUCCESSFUL 2000
#define CMD_PROC_UNKNOWN 2001

char getHelpChar();
char getQuitChar();

int processCommand(char cmd, int (*int_getter)());

#endif