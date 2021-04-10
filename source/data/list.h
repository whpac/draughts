#ifndef LIST_H
#define LIST_H

typedef struct List List;

List* listCreate();
void listDestroy(List* l, char free_content);

void listRemove(List* l, int index, char free_content);
void* listGet(List* l, int index);
void listSet(List* l, int index, void* new_content, char free_old);
int listAdd(List* l, void* content);

int listGetLength(List* l);

#endif