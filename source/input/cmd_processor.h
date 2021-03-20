#ifndef CMD_PROCESSOR_H
#define CMD_PROCESSOR_H

#define CMD_PROC_SUCCESSFUL 0
#define CMD_PROC_UNKNOWN 1
#define CMD_PROC_TO_FIELD_UNPLAYABLE 2

char getHelpChar();
char getQuitChar();

int processCommand(char cmd, int (*int_getter)());

#endif