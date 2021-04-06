#include<stddef.h>
#include<malloc.h>
#include "position.h"

struct Position {
    int row;
    int col;
    Pawn* pawn;
};

/**
 * Creates a new position descriptor.
 * @param row The row associated with a position
 * @param col The column associated with a position
 * @param pawn A pointer to the pawn at the position (may also be NULL)
 */
Position* positionCreate(int row, int col, Pawn* pawn){
    Position* pos = malloc(sizeof(Position));
    if(pos == NULL) return NULL;

    pos->row = row;
    pos->col = col;
    pos->pawn = pawn;
    return pos;
}

/**
 * Frees the memory occupied by the position descriptor.
 * @param pos The position descriptor
 */
void positionDestroy(Position* pos){
    if(pos == NULL) return;

    free(pos);
}

/**
 * Retrieves the row from the position
 * @param row The position descriptor
 */
int positionGetRow(Position* pos){
    if(pos == NULL) return -1;
    return pos->row;
}

/**
 * Retrieves the column from the position
 * @param pos The position descriptor
 */
int positionGetColumn(Position* pos){
    if(pos == NULL) return -1;
    return pos->col;
}

/**
 * Returns the pawn associated with the position
 * @param pos The position descriptor
 */
Pawn* positionGetPawn(Position* pos){
    if(pos == NULL) return NULL;
    return pos->pawn;
}