#ifndef QUEUE_H
#define QUEUE_H

typedef struct Queue Queue;

Queue* queueCreate();
void queueDestroy(Queue* s);
char queueIsEmpty(Queue* s);
void queuePush(Queue* s, void* e);
void* queuePop(Queue* s);
void* queuePeek(Queue* s);

#endif