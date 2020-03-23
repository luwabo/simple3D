#ifndef CR_GRAPHICS_H_
#define CR_GRAPHICS_H_

#include <valloc.h>
#include <malloc.h>

#include <pspgu.h>

#include <cr_texture.h>

// TODO: add batch-renderer (binary-searchtree with <texture, List<cr_graphics_mesh>>)
// TODO: for now draw-call for each mesh

typedef struct cr_graphics_vertex {
  float u, v;
  float x, y ,z;
} cr_graphics_vertex;

#define CR_GRAPHICS_VERTEX_TYPE (GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_INDEX_16BIT | GU_TRANSFORM_3D)

#define CR_GRAPHICS_SCR_WIDTH (480)
#define CR_GRAPHICS_SCR_HEIGHT (270)

#define CR_GRAPHICS_BUF_WIDTH (512)


typedef struct cr_graphics_mesh {

  cr_graphics_vertex* vertices;
  size_t numVertices;

  short* vertexIndices;

} cr_graphics_mesh;

typedef struct cr_graphics_renderObj {

  cr_graphics_mesh* mesh;

  cr_texture* texture;
  
  ScePspFVector3 rotate;
  ScePspFVector3 scale;
  ScePspFVector3 translate;

} cr_graphics_renderObj;


// init graphics-system
void cr_graphics_init (void);

// terminates graphics-system
void cr_graphics_term (void);

// draws a given mesh in the current draw-buffer
void cr_graphics_drawMesh (cr_graphics_renderObj* mesh);

// swaps buffer + syncs with display-hardware
void cr_graphics_present (void);

// clears the screen
void cr_graphics_clearScreen (void);

// start cr_graphics_context
void cr_graphics_start (void);

// end cr_graphics_context
void cr_graphics_end (void);

#endif // CR_GRAPHICS_H_
