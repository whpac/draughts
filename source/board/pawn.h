#ifndef PAWN_H
#define PAWN_H

typedef enum PawnColor { white, black } PawnColor;
typedef struct Pawn Pawn;

Pawn* createPawn(PawnColor color, char is_king);
void destroyPawn(Pawn* pawn);

PawnColor getPawnColor(Pawn* pawn);
char isPawnKing(Pawn* pawn);
void setIsPawnKing(Pawn* pawn, char is_king);

#endif