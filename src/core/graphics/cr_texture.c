#include "cr_texture.h"

#include <stdio.h>
#include <stdlib.h>

#include <png.h>
#include <valloc.h>

#include <pspkernel.h>
#include <pspgu.h>

#include <cr_maths.h>
#include <cr_graphics.h>

// ??
static inline void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
  return;
}

static int getTextureSize (int psm, int width, int height) {

  int bpp = 0;

  switch (psm) {
  case GU_PSM_T4:
    bpp = 4;
    break;
  case GU_PSM_T8:
    bpp = 8;
    break;
  case GU_PSM_5650:
  case GU_PSM_5551:
  case GU_PSM_4444:
  case GU_PSM_T16:
    bpp = 16;
    break;
  case GU_PSM_8888:
  case GU_PSM_T32:
    bpp = 32;
    break;
  default:
    bpp = 0;
    break;
  }

  return (bpp * width * height) / 8;
}

cr_texture* cr_texture_loadTEX (const char* path, int width, int height) {

  cr_texture* texture = (cr_texture*) malloc(sizeof(cr_texture));

  texture->ramData = texture->vramData = NULL;

  FILE* file = fopen(path, "rb");

  texture->psm = GU_PSM_8888;

  texture->width = width;
  texture->height = height;

  texture->pow2width = cr_maths_nextPow2(width);
  texture->pow2height = cr_maths_nextPow2(height);

  texture->alignedWidth = cr_maths_align(width, 16);

  // allocated 16-bytes aligned
  texture->ramData = memalign(16, texture->alignedWidth * height * 4);

  fread(texture->ramData, width * height * 4, 1, file);

  fclose(file);

  return texture;
}

cr_texture* cr_texture_loadPNG (const char* path) {
  return NULL;
}

void cr_texture_delete (cr_texture* texture) {

  if (texture->ramData != NULL) {
    free(texture->ramData);
  }

  if (texture->vramData != NULL) {
    vfree(texture->vramData);
  }

  free(texture);
}

void cr_texture_uploadVRAM (cr_texture* texture) {

  if (texture->vramData == NULL && texture->ramData != NULL) {

    void* __attribute__ ((aligned(16))) temp = valloc(getTextureSize(texture->psm, texture->width, texture->height));
    texture->vramData = temp;


    cr_graphics_start();

    sceGuCopyImage(texture->psm, 0, 0, texture->width, texture->height,
		   texture->alignedWidth, texture->ramData, 0, 0,
		   texture->alignedWidth, texture->vramData);

    cr_graphics_end();
  }
}

void cr_texture_removeVRAM (cr_texture* texture) {

  if (texture->vramData != NULL) {
    vfree(texture->vramData);
  }
}

void cr_texture_uploadRAM (cr_texture* texture) {

  if (texture->ramData == NULL && texture->vramData != NULL) {

    int textureSize = getTextureSize(texture->psm, texture->width, texture->height);

    texture->ramData = malloc(textureSize);

    unsigned char* dstPtr = (unsigned char*) texture->ramData;
    unsigned char* srcPtr = (unsigned char*) texture->vramData;

    for (int i = 0; i < textureSize; ++i) {
      dstPtr[i] = srcPtr[i];
    }
  }
}

void cr_texture_removeRAM (cr_texture* texture) {

  if (texture->ramData != NULL) {
    free(texture->ramData);
  }
}
