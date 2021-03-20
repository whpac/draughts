#include<stddef.h>
#include<malloc.h>
#include "pawn.h"

struct Pawn {
    PawnColor color;
    char is_king;
};

/**
 * Creates a new pawn in memory. If it's impossible to allocate memory, returns NULL
 * @param color The pawn color
 * @param is_king Whether the pawn is a king
 */
Pawn* createPawn(PawnColor color, char is_king){
    Pawn* pawn = malloc(sizeof(Pawn));
    if(pawn == NULL) return NULL;

    pawn->color = color;
    pawn->is_king = is_king;

    return pawn;
}

/**
 * Destroys the pawn
 * @param pawn A pawn to be destroyed
 */
void destroyPawn(Pawn* pawn){
    free(pawn);
}

/**
 * Returns the pawn color
 * @param pawn The pawn to check
 */
PawnColor getPawnColor(Pawn* pawn){
    return pawn->color;
}

/**
 * Returns whether the pawn is a king
 * @param pawn The pawn to check
 */
char isPawnKing(Pawn* pawn){
    return pawn->is_king;
}

/**
 * Transforms the pawn to a king
 * @param pawn The pawn to transpform
 */
void transformToKing(Pawn* pawn){
    pawn->is_king = 1;
}