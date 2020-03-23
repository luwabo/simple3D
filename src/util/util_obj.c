#include "util_obj.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include <util_list.h>
#include <util_map.h>

#define VERTEX_COORD ("v ")
#define TEXTURE_COORD ("vt ")
#define FACE ("f ")

#define LINE_START_SIZE 100

typedef struct vertexPos {
  float x, y, z;
} vertexPos;

typedef struct textureCoord {
  float u, v;
} textureCoord;

typedef struct indexReference {
  int textureIndex;
  int vertexIndex;
} indexReference;
 
 
static bool getLineType (char* line, size_t lineSize, char* type) {

  if (lineSize < 2) {
    return false;
  }

  return line[0] == type[0] && line[1] == type[1];
}

static int getLine (char* buffer, size_t* bufferSize, FILE* stream) {

  int readBytes = 0;
  char input;

  while (fread((void*) &input, 1, 1, stream) != 0) {

    *(buffer + readBytes) = input;

    if (++readBytes == *bufferSize) {
      *bufferSize *= 2;
      realloc(buffer, *bufferSize);
    }

    if (input == '\n') {
      *(buffer + readBytes - 1) = ' ';
      return readBytes;
    }

  }

  return -1;
}

static void getNumberPos (char** numPos, char* line, size_t lineSize) {

  bool inside = false;

  int j = 0;

  for (int i = 0; i < lineSize; ++i) {

    if (!inside && (isdigit(line[i]) || line[i] == '-')) {
      numPos[j++] = &line[i];
      inside = true;
    }

    else if (inside && !(isdigit(line[i]) || line[i] == '.')) {
      inside = false;
    }
  }
}

static void getVertexPos (util_list* list, char* line, size_t lineSize) {

  vertexPos* pos = (vertexPos*) malloc(sizeof(vertexPos));

  char* numPos [3];

  getNumberPos(&numPos[0], line, lineSize);

  pos->x = strtof(numPos[0], NULL);
  pos->y = strtof(numPos[1], NULL);
  pos->z = strtof(numPos[2], NULL);

  util_list_addEnd(list, (void*) pos);
}

static void getTextureCoord (util_list* list, char* line, size_t lineSize) {

  textureCoord* coord = (textureCoord*) malloc(sizeof(textureCoord));

  char* numPos[2];

  getNumberPos(&numPos[0], line, lineSize);

  coord->u = strtof(numPos[0], NULL);
  coord->v = strtof(numPos[1], NULL);

  util_list_addEnd(list, (void*) coord);
}


static void getIndices (util_list* indices, util_map* indexRef, char* line, size_t lineSize) {

  char* numPos[6];

  getNumberPos(&numPos[0], line, lineSize);

  indexReference tmpKey;

  for (int i = 0; i < 6; i += 2) {
    tmpKey.vertexIndex = atoi(numPos[i]) - 1;
    tmpKey.textureIndex = atoi(numPos[i+1]) - 1;

    int* indexPos = NULL;

    if ((indexPos = (int*) util_map_get(indexRef, (void*) &tmpKey)) == NULL) {
      indexReference* newKey = (indexReference*) malloc(sizeof(indexReference));

      indexPos = (int*) malloc(sizeof(int));

      newKey->vertexIndex = tmpKey.vertexIndex;
      newKey->textureIndex = tmpKey.textureIndex;

      *indexPos = util_map_getSize(indexRef);

      util_map_put(indexRef, (void*) newKey, (void*) indexPos);
    }

    int* tmp = malloc(sizeof(int));
    *tmp = *indexPos;

    util_list_addEnd(indices, tmp);
  }
}

static int compareIndices (void* int1, void* int2) {

  indexReference* base1 = (indexReference*) int1;
  indexReference* base2 = (indexReference*) int2;

  int cmp1 = base1->vertexIndex - base2->vertexIndex;

  if (cmp1 == 0) {
    return base1->textureIndex - base2->textureIndex;
  }

  return cmp1;
}

cr_graphics_mesh* util_obj_loadMesh (char* path) {

  cr_graphics_mesh* newMesh = (cr_graphics_mesh*) malloc(sizeof(cr_graphics_mesh));

  newMesh->vertices = NULL;
  newMesh->numVertices = 0;
  newMesh->vertexIndices = NULL;

  FILE* file = fopen(path, "rb");

  // read file line-by-line
  char* line = (char*) malloc(sizeof(char) * LINE_START_SIZE);
  size_t lineSize = LINE_START_SIZE;

  util_list* vertexPosList = util_list_createList(free);
  util_list* textureCoordList = util_list_createList(free);
  util_list* indicesList = util_list_createList(free);
  util_map* indexRefMap = util_map_createMap(compareIndices, free, free);

  while (getLine(line, &lineSize, file) != -1) {

    // analyse it (vertex/texture/faces)

    if (getLineType(line, lineSize, VERTEX_COORD)) {
      getVertexPos(vertexPosList, line, lineSize);
    }

    else if (getLineType(line, lineSize, TEXTURE_COORD)) {
      getTextureCoord(textureCoordList, line, lineSize);
    }

    else if (getLineType(line, lineSize, FACE)) {
      getIndices(indicesList, indexRefMap, line, lineSize);
    }

    else {
      // unsupported
    }
  }

  newMesh->vertices = (cr_graphics_vertex*) malloc(util_map_getSize(indexRefMap));

  util_map_traversalReset(indexRefMap);

  void* key;
  void* val;

  util_map_traversalInOrder(indexRefMap);

  while ((key = util_map_getCrntKey(indexRefMap)) != NULL && (val = util_map_getCrntVal(indexRefMap)) != NULL) {

    indexReference* ref = (indexReference*) key;

    textureCoord* texCoord = (textureCoord*) util_list_get(textureCoordList, ref->textureIndex);

    newMesh->vertices[*((int*) val)].u = texCoord->u;
    newMesh->vertices[*((int*) val)].v = texCoord->v;

    vertexPos* vertex = (vertexPos*) util_list_get(vertexPosList, ref->vertexIndex);

    newMesh->vertices[*((int*) val)].x = vertex->x;
    newMesh->vertices[*((int*) val)].y = vertex->y;
    newMesh->vertices[*((int*) val)].z = vertex->z;

    util_map_traversalInOrder(indexRefMap);
  }

  newMesh->numVertices = util_list_getSize(indicesList);

  newMesh->vertexIndices = (short*) malloc(sizeof(short) * newMesh->numVertices);

  for (int i = 0; i < newMesh->numVertices; ++i) {
    newMesh->vertexIndices[i] = *((short*) util_list_get(indicesList, i));
  }

  return newMesh;
}

void util_obj_deleteMesh (cr_graphics_mesh* mesh) {

  free(mesh->vertices);
  free(mesh->vertexIndices);
  free(mesh);
}
