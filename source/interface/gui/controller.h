#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../../data/list.h"

void guiInitController();
void guiDeinitController();
void guiPaintBoard();

void guiReloadBoard();
void guiMoveCursor(int drow, int dcol);

List* guiGetMarkers();

#endif