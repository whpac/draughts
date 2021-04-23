#include<stddef.h>
#include<malloc.h>
#include "stack.h"

typedef struct StackElem {
    void* previous;
    void* elem;
} StackElem;

struct Stack {
    StackElem* top;
};

/** Creates an empty stack */
Stack* stackCreate(){
    Stack* s = malloc(sizeof(Stack));
    if(s == NULL) return NULL;

    s->top = NULL;
    return s;
}

/**
 * Frees memory used by the stack. Doesn't free memory occupied by the particular elements.
 * @param s The stack to destroy
 */
void stackDestroy(Stack* s){
    if(s == NULL) return;

    while(!stackIsEmpty(s)){
        stackPop(s);
    }
    free(s);
}

/**
 * Checks if the stack is empty
 * @param s The stack to check
 */
char stackIsEmpty(Stack* s){
    if(s == NULL) return 1;
    return (s->top == NULL);
}

/**
 * Pushes a pointer onto the stack
 * @param s The stack to push the element onto
 * @param e A pointer to the element to be pushed
 */
void stackPush(Stack* s, void* e){
    StackElem* se = malloc(sizeof(StackElem));
    if(se == NULL) return;

    se->elem = e;
    se->previous = s->top;
    s->top = se;
}

/**
 * Pops a pointer from the stack. If stack is empty, returns NULL.
 * @param s The stack to pop an element from
 */
void* stackPop(Stack* s){
    if(s->top == NULL) return NULL;

    StackElem* se = s->top;
    s->top = se->previous;

    void* elem = se->elem;
    free(se);

    return elem;
}

/**
 * Peeks a pointer from the stack. If stack is empty, returns NULL. 
 * Doesn't remove the topmost element from the stack.
 * @param s The stack to peek an element from
 */
void* stackPeek(Stack* s){
    if(s->top == NULL) return NULL;

    StackElem* se = s->top;
    return se->elem;
}