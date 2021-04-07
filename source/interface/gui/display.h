#ifndef GUI_DISPLAY_H
#define GUI_DISPLAY_H

#include<allegro5/allegro5.h>

char guiInit();
void guiDestroy();

void guiReloadBoard();
void guiPaintBoard();

ALLEGRO_TIMER* guiGetTimer();
ALLEGRO_EVENT_QUEUE* guiGetEventQueue();

#endif