#include<malloc.h>
#include "list.h"

typedef struct ListElement ListElement;

struct List {
    ListElement* first;
};

struct ListElement {
    void* content;
    ListElement* next;
};

ListElement* listGetElement(List* l, int index);
int listAddElement(List* l, ListElement* element);

/** Creates an empty list */
List* listCreate(){
    List* l = malloc(sizeof(List));
    if(l == NULL) return NULL;

    l->first = NULL;
    return l;
}

/**
 * Destroys the list and frees the memory
 * @param l The list to destroy
 * @param free_content Whether to free the space occupied by contents of each element
 */
void listDestroy(List* l, char free_content){
    while(l->first != NULL){
        listRemoveElement(l, 0, free_content);
    }
}

/**
 * Retrieves the specified element from the list
 * @param l The list
 * @param index The desired element's index
 */
ListElement* listGetElement(List* l, int index){
    ListElement* el = l->first;

    for(int i = 0; i < index; i++){
        if(el == NULL) break;
        el = el->next;
    }

    return el;
}

/**
 * Adds the passed element to the end of the list. Returns the index of newly-added element
 * @param l The list
 * @param element The element to add
 */
int listAddElement(List* l, ListElement* element){
    element->next = NULL;
    if(l->first == NULL){
        l->first = element;
        return 0;
    }else{
        ListElement* le = l->first;
        int index = 1;
        while(le->next != NULL){
            le = le->next;
            index++;
        }
        le->next = element;
        return index;
    }
}

/**
 * Removes the element at the specified index
 * @param l The list
 * @param index Index of the element to remove
 * @param free_content Whether to free the space occupied by the element's content
 */
void listRemoveElement(List* l, int index, char free_content){
    ListElement** link_to_removed;
    if(index <= 0){
        if(l->first == NULL) return;
        link_to_removed = &(l->first);
    }else{
        ListElement* le = listGetElement(l, index-1);
        if(le == NULL) return;
        if(le->next == NULL) return;
        link_to_removed = &(le->next);
    }

    ListElement* removed = *link_to_removed;
    *link_to_removed = removed->next;

    if(free_content) free(removed->content);
    free(removed);
}

/**
 * Retrieves the data stored at the specified index
 * @param l The list
 * @param index The desired element's index
 */
void* listGet(List* l, int index){
    ListElement* el = listGetElement(l, index);

    if(el == NULL) return NULL;
    return el->content;
}

/**
 * Sets the data stored at the specified index
 * @param l The list
 * @param index The new element's index
 * @param new_content A pointer to the new element's content
 * @param free_old Whether to free memory used by the old contents
 */
void listSet(List* l, int index, void* new_content, char free_old){
    ListElement* el = listGetElement(l, index);

    if(el == NULL) return;
    if(free_old) free(el->content);

    el->content = new_content;
}

/**
 * Adds the passed value to the end of the list
 * @param l The list
 * @param content The value to be added
 */
int listAdd(List* l, void* content){
    ListElement* le = malloc(sizeof(ListElement));
    if(le == NULL) return -1;
    le->content = content;

    listAddElement(l, le);
}
