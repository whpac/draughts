#ifndef GUI_DISPLAY_H
#define GUI_DISPLAY_H

#include<allegro5/allegro5.h>
#include<allegro5/allegro_font.h>

char guiInit();
void guiDestroy();
void guiPaint();

ALLEGRO_TIMER* guiGetTimer();
ALLEGRO_EVENT_QUEUE* guiGetEventQueue();
ALLEGRO_FONT* guiGetFont();

#endif