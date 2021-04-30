#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../../data/list.h"

void guiInitController();
void guiDeinitController();
void guiPaintBoard();

char guiIsWelcomeShown();
void guiHideWelcome();

void guiMoveCursor(int drow, int dcol);

void guiSelectCurrentField(char freeze);
char guiIsFieldSelected();
char guiIsCurrentFieldSelected();

int guiAttemptMoveFromSelectedToCursor();
void guiAttemptUndo();

char isGameOver();
List* guiGetMarkers();

#endif