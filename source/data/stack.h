#ifndef STACK_H
#define STACK_H

typedef struct Stack Stack;

Stack* stackCreate();
void stackDestroy(Stack* s);
char stackIsEmpty(Stack* s);
void stackPush(Stack* s, void* e);
void* stackPop(Stack* s);
void* stackPeek(Stack* s);

#endif