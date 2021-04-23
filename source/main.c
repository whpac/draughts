#include <stdio.h>
#include <stdlib.h>

#include "board/pawn.h"
#include "board/board.h"
#include "interface/text/input.h"
#include "interface/gui/display.h"
#include "interface/gui/keyboard.h"

int main()
{
    initBoard();

    createStartLayout();

    if(guiInit()){
        guiBeginInputLoop();
        guiDestroy();
    }else{
        // A fallback for GUI init errors
        cliBeginInputLoop();
    }

    destroyBoard();
    return 0;
}
