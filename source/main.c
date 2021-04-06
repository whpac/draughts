#include <stdio.h>
#include <stdlib.h>

#include "board/pawn.h"
#include "board/board.h"
#include "display/console.h"
#include "input/text.h"

int main()
{
    initBoard();

    createStartLayout();
    beginInputLoop(&displayBoard);

    destroyBoard();
    return 0;
}
