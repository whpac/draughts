#ifndef PAWN_H
#define PAWN_H

typedef enum PawnColor { white, black } PawnColor;
typedef struct Pawn Pawn;

Pawn* createPawn(PawnColor color, char is_king);
PawnColor getPawnColor(Pawn* pawn);
char isPawnKing(Pawn* pawn);

#endif