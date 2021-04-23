#ifndef LOG_ACTION_H
#define LOG_ACTION_H

#include "../board/pawn.h"

typedef enum LogActionType LogActionType;
enum LogActionType { unknown, move, undo };

typedef struct LogAction LogAction;
struct LogAction {
    LogActionType actionType;
    int rfrom;
    int cfrom;
    int rto;
    int cto;
    PawnColor player;
};

LogAction* logActionCreate();
void logActionDestroy(LogAction* la);

#endif