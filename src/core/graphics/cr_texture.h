#ifndef CR_TEXTURE_H_
#define CR_TEXTURE_H_


typedef struct cr_texture {

  unsigned short width, height;
  unsigned short alignedWidth;

  unsigned short pow2height, pow2width;

  int psm;

  void* ramData;
  void* vramData;

} cr_texture;

// load texture from PNG
cr_texture* cr_texture_loadPNG (const char* path); 

// load texture from custom TEX-format
cr_texture* cr_texture_loadTEX (const char* path, int width, int height);

// deletes texture
void cr_texture_delete (cr_texture* texture);

/// upload texture into VRAM
void cr_texture_uploadVRAM (cr_texture* texture);

// removes texture from VRAM
void cr_texture_removeVRAM (cr_texture* texture);

/// upload texture into RAM
void cr_texture_uploadRAM (cr_texture* texture);

// removes texture from RAM
void cr_texture_removeRAM (cr_texture* texture);


#endif // CR_TEXTURE_H_
