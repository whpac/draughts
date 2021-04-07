#ifndef GUI_PAINTER_H
#define GUI_PAINTER_H

#include "../../board/pawn.h"

void paintBoard(Pawn** buffer, int board_size, int crow, int ccol);
float getFieldWidth();

#endif