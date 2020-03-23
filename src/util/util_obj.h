#ifndef UTIL_OBJ_H_
#define UTIL_OBJ_H_

#include <cr_graphics.h>

// data must be an .obj only containing: counter-clockwise, vertex-positions and texture-coordinates
cr_graphics_mesh* util_obj_loadMesh (char* path);

void util_obj_deleteMesh (cr_graphics_mesh* mesh);



#endif // UTIL_OBJ_H_
