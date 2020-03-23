#include "util_list.h"

#include <stdlib.h>


#include <pspdebug.h>

util_list* util_list_createList (util_freeValue freeValue) {
  util_list* newList = (util_list*) malloc(sizeof(util_list));

  newList->numElements = 0;
  newList->head = newList->tail = NULL;
  newList->freeValue = freeValue;

  return newList;
}

void util_list_removeList (util_list* list) {

  for (util_list_node* crnt = list->head; crnt != NULL;) {
    util_list_node* next = crnt->next;

    list->freeValue(crnt->data);
    free(crnt);

    crnt = next;
  }

  list->numElements = 0;
  list->head = list->tail = NULL;
  list->freeValue = NULL;
}

void util_list_addEnd (util_list* list, void* data) {


  util_list_node* newNode = (util_list_node*) malloc(sizeof(util_list_node));

  newNode->data = data;
  newNode->prev = newNode->next = NULL;

  if (list->numElements++ == 1) {
    list->head = list->tail = newNode;
    return;
  }

  list->tail->next = newNode;
  newNode->prev = list->tail;
  list->tail = newNode;
}

void util_list_removeEnd (util_list* list) {

  if (list->numElements == 0) {
    return;
  }

  util_list_node* prev = list->tail->prev;

  list->freeValue(list->tail->data);

  free(list->tail);

  list->tail = prev;

  if (--list->numElements == 0) {
    list->tail = list->head = NULL;
  }

  else {
    list->tail->next = NULL; 
  }
}

void util_list_removeFront (util_list* list) {

  if (list->numElements == 0) {
    return;
  }

  util_list_node* next = list->head->next;

  list->freeValue(list->head->data);

  free(list->head);

  list->head = next;

  if (--list->numElements == 0) {
    list->head = list->tail = NULL;
  }

  else {
    list->head->prev = NULL;
  }
}

void* util_list_get (util_list* list, size_t i) {

  if (i < 0 || i >= list->numElements) {
    return NULL;
  }


  util_list_node* crnt = list->head;

  for (int j = 0; j < i; ++j, crnt = crnt->next);

  if (crnt != NULL) {
    return crnt->data;
  }

  return NULL;
}

size_t util_list_getSize (util_list* list) {
  return list->numElements;
}
