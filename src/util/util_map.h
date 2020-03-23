#ifndef UTIL_MAP_H_
#define UTIL_MAP_H_

#include <stdlib.h>
#include <stdbool.h>

#include "util_mem.h"

// TODO: re-implement map with RB-trees

typedef struct util_map_node {
  void* key;
  void* value;
  struct util_map_node* parent;
  struct util_map_node* right;
  struct util_map_node* left;

} util_map_node;




typedef struct {
  size_t size;

  util_map_node* root;
  util_map_node* crnt;

  util_compare* compare;
  util_freeKey* freeKey;
  util_freeValue* freeValue;

} util_map;

// creates a map
util_map* util_map_createMap (util_compare compare,
				    util_freeKey freeKey,
				    util_freeValue freeValue);

// removes a map
void util_map_removeMap (util_map* map);

// adds (key,value) pair
void util_map_put (util_map* map, void* key, void* value);

// gets value to key
void* util_map_get (util_map* map, void* key);

// removes (key,value) pair
void util_map_remove (util_map* map, void* key);

// gets the size of the map
int util_map_getSize (util_map* map);

void* util_map_getCrntKey (util_map* map);

void* util_map_getCrntVal (util_map* map);

void util_map_traversalReset (util_map* map);
void util_map_traversalInOrder (util_map* map);

#endif // CR_UTIL_MAP_H_
