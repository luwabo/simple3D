#ifndef UTIL_LIST_H_
#define UTIL_LIST_H_

#include "util_mem.h"

#include <stdlib.h>

typedef struct util_list_node {
  void* data;

  struct util_list_node* prev;
  struct util_list_node* next;

} util_list_node;

typedef struct util_list {

  size_t numElements;

  util_list_node* head;
  util_list_node* tail;

  util_freeValue* freeValue;

} util_list;

util_list* util_list_createList (util_freeValue freeValue);

void util_list_removeList (util_list* list);

void util_list_addEnd (util_list* list, void* data);

void util_list_removeEnd (util_list* list);

void util_list_removeFront (util_list* list);

void* util_list_get (util_list* list, size_t i);

size_t util_list_getSize (util_list* list);

#endif // UTIL_LIST_H_
