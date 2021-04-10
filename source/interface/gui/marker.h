#ifndef MARKER_H
#define MARKER_H

enum MarkerColor { red, green, gold, gray };
enum MarkerStyle { frame, point };
typedef enum MarkerColor MarkerColor;
typedef enum MarkerStyle MarkerStyle;

typedef struct Marker {
    int row;
    int col;
    MarkerColor color;
    MarkerStyle style;
} Marker;

Marker* markerCreate();
void markerDestroy(Marker* m);

#endif