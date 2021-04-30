#include<malloc.h>
#include "log_action.h"

/** Creates a pointer to a new LogAction */
LogAction* logActionCreate(){
    LogAction* la = malloc(sizeof(LogAction));

    la->actionType = unknown;
    la->rfrom = -1;
    la->cfrom = -1;
    la->rto = -1;
    la->cto = -1;
    la->player = white;

    return la;
}

/**
 * Frees a memory occupied by the LogAction
 * @param la The LogAction
 */
void logActionDestroy(LogAction* la){
    free(la);
}