#include "cr_graphics.h"

#include <stdbool.h>

#include <pspgum.h>
#include <pspdisplay.h>

static unsigned int __attribute__((aligned(16))) list[262144];

static bool initialised = false;

static void* dispBuffer;
static void* drawBuffer;
static void* depthBuffer;

void cr_graphics_init (void) {
  
  if (initialised) {
    return;
  }

  initialised = true;

  dispBuffer = valloc(vgetMemorySize(CR_GRAPHICS_BUF_WIDTH, CR_GRAPHICS_SCR_HEIGHT, GU_PSM_8888));
  drawBuffer = valloc(vgetMemorySize(CR_GRAPHICS_BUF_WIDTH, CR_GRAPHICS_SCR_HEIGHT, GU_PSM_8888));
  depthBuffer = valloc(vgetMemorySize(CR_GRAPHICS_BUF_WIDTH, CR_GRAPHICS_SCR_HEIGHT, GU_PSM_4444));

  sceGuInit();

  sceGuStart(GU_DIRECT, list);

  sceGuDrawBuffer(GU_PSM_8888, vGuPointer(drawBuffer), CR_GRAPHICS_BUF_WIDTH);
  sceGuDispBuffer(CR_GRAPHICS_SCR_WIDTH, CR_GRAPHICS_SCR_HEIGHT, vGuPointer(dispBuffer), CR_GRAPHICS_BUF_WIDTH);
  sceGuDepthBuffer(vGuPointer(depthBuffer), CR_GRAPHICS_BUF_WIDTH);

  sceGuOffset(2048 - (CR_GRAPHICS_SCR_WIDTH/2), 2048 - (CR_GRAPHICS_SCR_HEIGHT/2));
  sceGuViewport(2048, 2048, CR_GRAPHICS_SCR_WIDTH, CR_GRAPHICS_SCR_HEIGHT);
  sceGuDepthRange(65335, 0);
  sceGuScissor(0, 0, CR_GRAPHICS_SCR_WIDTH, CR_GRAPHICS_SCR_HEIGHT);
  sceGuEnable(GU_SCISSOR_TEST);

  sceGuEnable(GU_TEXTURE_2D);
  sceGuAlphaFunc(GU_GREATER, 0, 0x000000ff);
  sceGuEnable(GU_ALPHA_TEST);

  sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
  sceGuTexScale(1.0f, 1.0f);
  sceGuTexOffset(0.0f, 0.0f);
  sceGuTexWrap(GU_REPEAT, GU_REPEAT);
  sceGuAmbientColor(0x00000000);

  sceGuEnable(GU_LIGHTING);
  sceGuEnable(GU_LIGHT0);
  sceGuEnable(GU_LIGHT1);
  sceGuEnable(GU_LIGHT2);
  sceGuEnable(GU_LIGHT3);

  sceGuTexFilter(GU_NEAREST, GU_NEAREST);

  sceGuShadeModel(GU_SMOOTH);

  sceGuDepthFunc(GU_LEQUAL);
  sceGuDepthFunc(GU_GEQUAL);
  sceGuEnable(GU_DEPTH_TEST);

  sceGuFrontFace(GU_CW);

  sceGuEnable(GU_CULL_FACE);
  sceGuEnable(GU_CLIP_PLANES);


  sceGuFinish();
  sceGuSync(0, 0);

  sceDisplayWaitVblankStart();

  sceGuDisplay(GU_TRUE);
}


void cr_graphics_term (void) {
  
  vfree(dispBuffer);
  vfree(drawBuffer);
  vfree(depthBuffer);

  sceGuTerm();
}

void cr_graphics_drawMesh (cr_graphics_renderObj* renderObj) {
  
  sceGumMatrixMode(GU_MODEL);
  sceGumLoadIdentity();

  sceGumRotateXYZ(&renderObj->rotate);
  sceGumScale(&renderObj->scale);
  sceGumTranslate(&renderObj->translate);

  // texture-setup
  if (renderObj->texture != NULL) {
    
    sceGuTexMode(renderObj->texture->psm, 0, 0, GU_FALSE);

    // tries to use texture in VRAM first
    void* textureData;

    if (renderObj->texture->vramData != NULL) {
      textureData = renderObj->texture->vramData;
    }

    else {
      textureData = renderObj->texture->ramData;
    }

    sceGuTexImage(0, renderObj->texture->pow2width,
  		  renderObj->texture->pow2height, renderObj->texture->alignedWidth, textureData);
  }

  sceGumDrawArray(GU_TRIANGLES, CR_GRAPHICS_VERTEX_TYPE,
  		  renderObj->mesh->numVertices, renderObj->mesh->vertexIndices, renderObj->mesh->vertices);
}

void cr_graphics_present (void) {

  sceGuSync(0,0);
  sceDisplayWaitVblankStart();
  sceGuSwapBuffers();
}

void cr_graphics_clearScreen (void) {

  //sceGuClearColor(0x00000000);
  sceGuClearColor(0xff554433);
  sceGuClearDepth(0);
  sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
}

void cr_graphics_start (void) {
  
  sceGuStart(GU_DIRECT, list);
}

void cr_graphics_end (void) {
  
  sceGuFinish();
}
