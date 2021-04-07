#include <stdio.h>
#include <stdlib.h>

#include "board/pawn.h"
#include "board/board.h"
#include "interface/text/input.h"

int main()
{
    initBoard();

    createStartLayout();
    beginInputLoop();

    destroyBoard();
    return 0;
}
