#include<malloc.h>
#include "marker.h"

/** Creates a new marker */
Marker* markerCreate(){
    return malloc(sizeof(Marker));
}

/** Destroys the marker */
void markerDestroy(Marker* m){
    free(m);
}