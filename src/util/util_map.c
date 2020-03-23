#include "util_map.h"


static void* getNode (util_map* map, void* key) {

  util_map_node* crnt = map->root;

  int cmpVal;

  while (crnt != NULL && (cmpVal = map->compare(key, crnt->key)) != 0) {
    crnt = (cmpVal < 0) ? (crnt->left) : (crnt->right);
  }

  return crnt;
}

static void* getMin (util_map_node* node) {

  util_map_node* crnt = node;

  while (crnt->left != NULL) {
    crnt = crnt->left;
  }

  return crnt;
}

static void* getSuccessor (util_map_node* node) {

  if (node->right != NULL) {
    return getMin(node->right);
  }

  util_map_node* crnt = node;

  while (crnt != NULL && crnt->parent->left != crnt) {
    crnt = crnt->parent;
  }

  if (crnt == NULL) {
    return NULL;
  }

  return crnt->parent;
}

static void transplant (util_map* map, util_map_node* node) {

  util_map_node* onlyChild = (node->left != NULL) ? (node->left) : (node->right);

  if (node->parent != NULL) {
    if (node->parent->left == node) {
	node->parent->left = onlyChild;
    }

    else {
	node->parent->right = onlyChild;
    }
  }

  onlyChild->parent = node->parent;

  if (node == map->root) {
    map->root = onlyChild;
  }

  map->freeKey(node->key);
  map->freeValue(node->value);

  free(node);
}

static int numChildren (util_map_node* node) {

  int children = 0;

  if (node->left != NULL) {
    ++children;
  }

  if (node->right != NULL) {
    ++children;
  }

  return children;
}

static void deleteNoChild (util_map* map, util_map_node* node) {

  map->freeKey(node->key);
  map->freeValue(node->value);


  if (map->size == 1) {
    map->root = NULL;
  }

  else if (node->parent != NULL) {

    if (node->parent->left == node) {
      node->parent->left = NULL;
    }

    else {
      node->parent->right = NULL;
    }
  }

  free(node);
}

static void deleteOneChild (util_map* map, util_map_node* node) {

  transplant(map, node);
}

static void deleteTwoChildren (util_map* map, util_map_node* node) {

  util_map_node* succ = getSuccessor(node);

  void* nodeKey = node->key;
  void* nodeValue = node->value;

  node->key = succ->key;
  node->value = succ->value;

  succ->key = nodeKey;
  succ->value = nodeValue;

  util_map_remove(map, succ);
}


util_map* util_map_createMap (util_compare compare,
				    util_freeKey freeKey,
				    util_freeValue freeValue) {

  util_map* newMap = (util_map*) malloc(sizeof(util_map));

  newMap->root = NULL;
  newMap->crnt = NULL;
  newMap->size = 0;


  newMap->compare = compare;
  newMap->freeKey = freeKey;
  newMap->freeValue = freeValue;

  return newMap;
}

void util_map_traversalReset (util_map* map) {
  
  map->crnt = getMin(map->root);
}

void util_map_traversalInOrder (util_map* map) {

  map->crnt = getSuccessor(map->crnt);
}

void* util_map_getCrntKey (util_map* map) {
  if (map->crnt == NULL) {
    return NULL;
  }

  return map->crnt->key;
}

void* util_map_getCrntVal (util_map* map) {
  if (map->crnt == NULL) {
    return NULL;
  }
  
  return map->crnt->value;
}

static void deleteRecTree (util_map* map, util_map_node* node) {

  if (node->left != NULL) {
    deleteRecTree(map, node->left);
  }

  if (node->right != NULL) {
    deleteRecTree(map, node->right);
  }

  map->freeKey(node->key);
  map->freeValue(node->value);
  free(node);
}
 
void util_map_removeMap (util_map* map) {

  deleteRecTree(map, map->root);

  map->size = 0;
  map->root = map->crnt = NULL;

  map->compare = NULL;
  map->freeKey = NULL;
  map->freeValue = NULL;
}

void util_map_put (util_map* map, void* key, void* value) {

  if (map->size <= 0) {

    util_map_node* newNode = (util_map_node*) malloc(sizeof(util_map_node));

    newNode->key = key;
    newNode->value = value;
    newNode->parent = NULL;
    newNode->left = newNode->right = NULL;

    map->root = newNode;
    map->size++;

    return;
  }

  util_map_node* prev = map->root;
  util_map_node* crnt = map->root;

  int cmpVal = 0;

  while (crnt != NULL && (cmpVal = map->compare(key, crnt->key)) != 0) {
    prev = crnt;
    crnt = (cmpVal < 0) ? (crnt->left) : (crnt->right);
  }

  if (cmpVal == 0) {
    map->freeValue(crnt->value);
    crnt->value = value;
  }

  else {

    util_map_node* newNode = (util_map_node*) malloc(sizeof(util_map_node));

    newNode->parent = prev;
    newNode->left = newNode->right = NULL;
    newNode->key = key;
    newNode->value = value;

    map->size++;

    if (cmpVal < 0) {
      prev->left = newNode;
    }

    else {
      prev->right = newNode;
    }
  }
}



void* util_map_get (util_map* map, void* key) {

  util_map_node* node = getNode(map, key);

  if (node == NULL) {
    return NULL;
  }

  return node->value;
}

void util_map_remove (util_map* map, void* key) {

  util_map_node* node = getNode(map, key);

  if (node != NULL) {

    map->size--;

    switch (numChildren(node)) {
    case 0:
      deleteNoChild(map, node);
      break;
    case 1:
      deleteOneChild(map, node);
      break;
    case 2:
      deleteTwoChildren(map, node);
      break;
    }
  }
}

int util_map_getSize (util_map* map) {
  return map->size;
}
