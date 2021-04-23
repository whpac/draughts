#include<stddef.h>
#include<malloc.h>
#include "queue.h"

typedef struct QueueElem {
    void* next;
    void* elem;
} QueueElem;

struct Queue {
    QueueElem* base;
    QueueElem* top;
};

/** Creates an empty queue */
Queue* queueCreate(){
    Queue* q = malloc(sizeof(Queue));
    if(q == NULL) return NULL;

    q->base = NULL;
    q->top = NULL;
    return q;
}

/**
 * Frees memory used by the queue. Doesn't free memory occupied by the particular elements.
 * @param q The queue to destroy
 */
void queueDestroy(Queue* q){
    if(q == NULL) return;

    while(!queueIsEmpty(q)){
        queuePop(q);
    }
    free(q);
}

/**
 * Checks if the queue is empty
 * @param s The queue to check
 */
char queueIsEmpty(Queue* q){
    if(q == NULL) return 1;
    return (q->top == NULL);
}

/**
 * Pushes a pointer onto the queue
 * @param q The queue to push the element onto
 * @param e A pointer to the element to be pushed
 */
void queuePush(Queue* q, void* e){
    QueueElem* qe = malloc(sizeof(QueueElem));
    if(qe == NULL) return;

    if(q->base == NULL) q->base = qe;
    if(q->top != NULL) q->top->next = qe;

    qe->elem = e;
    qe->next = NULL;
    q->top = qe;
}

/**
 * Pops a pointer from the queue. If queue is empty, returns NULL.
 * @param q The queue to pop an element from
 */
void* queuePop(Queue* q){
    if(q->base == NULL) return NULL;

    QueueElem* qe = q->base;
    q->base = qe->next;

    if(q->base == NULL) q->top = NULL;

    void* elem = qe->elem;
    free(qe);

    return elem;
}

/**
 * Peeks a pointer from the queue. If queue is empty, returns NULL. 
 * Doesn't remove the first element from the queue.
 * @param q The queue to peek an element from
 */
void* queuePeek(Queue* q){
    if(q->base == NULL) return NULL;

    QueueElem* qe = q->base;
    return qe->elem;
}