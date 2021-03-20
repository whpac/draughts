#include <stdio.h>
#include <stdlib.h>

#include "board/pawn.h"
#include "board/board.h"

int main()
{
    initBoard();

    Pawn* p = createPawn(white, 0);
    printf("Color: %d, King: %d\n", getPawnColor(p), isPawnKing(p));

    return 0;
}
