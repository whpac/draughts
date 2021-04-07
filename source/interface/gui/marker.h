#ifndef MARKER_H
#define MARKER_H

enum MarkerColor { red, green, gold };
typedef enum MarkerColor MarkerColor;

typedef struct Marker {
    int row;
    int col;
    MarkerColor color;
} Marker;

Marker* markerCreate();
void markerDestroy(Marker* m);

#endif