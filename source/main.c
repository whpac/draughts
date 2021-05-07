#include <string.h>

#include "board/pawn.h"
#include "board/board.h"
#include "interface/text/input.h"
#include "interface/gui/display.h"
#include "interface/gui/keyboard.h"
#include "log/logger.h"

int main(int argc, char** argv)
{
    char use_gui = 1;
    if(argc >= 2){
        // If the first parameter is "text", don't display the window
        use_gui = strcmp(argv[1], "text");
    }

    logInit();
    initBoard();

    createStartLayout();

    if(use_gui && guiInit()){
        guiBeginInputLoop();
        guiDestroy();
    }else{
        // A fallback for GUI init errors
        cliBeginInputLoop();
    }

    destroyBoard();

    logDeinit();
    return 0;
}
