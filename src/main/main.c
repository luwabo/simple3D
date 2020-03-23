#include <pspkernel.h>
#include <stdlib.h>

#include <cr_exit_callback.h>
#include <cr_ctrl.h>

#include <cr_graphics.h>
#include <cr_camera.h>

#include <cr_texture.h>
#include <util_obj.h>


#include <pspgu.h>
#include <math.h>
#include "../teapot.h"
#include <time.h>
#include <stdlib.h>

PSP_MODULE_INFO("TESTING", 0, 1, 1);

PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);


cr_graphics_renderObj loadTeapot () {
  cr_graphics_renderObj teapotObj;
  teapotObj.mesh = (cr_graphics_mesh*) malloc(sizeof(cr_graphics_mesh));

  teapotObj.mesh->numVertices = teapot_count / 3;
  teapotObj.mesh->vertices = (cr_graphics_vertex*) malloc(sizeof(cr_graphics_vertex) * teapotObj.mesh->numVertices);

  int j = 0;

  srand(time(NULL));

  for (int i = 0; i < teapot_count/3; ++i) {
    teapotObj.mesh->vertices[i].x = teapot[j++];
    teapotObj.mesh->vertices[i].y = teapot[j++];
    teapotObj.mesh->vertices[i].z = teapot[j++];

    teapotObj.mesh->vertices[i].u = 0.5f;
    teapotObj.mesh->vertices[i].v = 0.5f;
  }

  teapotObj.mesh->vertexIndices = malloc (sizeof(short) * teapot_count/3);

  for (int i = 0; i < teapot_count/3; ++i) {
    teapotObj.mesh->vertexIndices[i] = i;
  }

  teapotObj.texture = cr_texture_loadTEX("./res/textures/test.tex", 64, 64);

  teapotObj.rotate.x = 0.0f;
  teapotObj.rotate.y = 0.0f;
  teapotObj.rotate.z = 0.0f;

  teapotObj.scale.x = 1.0f;
  teapotObj.scale.y = 1.0f;
  teapotObj.scale.z = 1.0f;

  teapotObj.translate.x = 0.0f;
  teapotObj.translate.y = 0.0f;
  teapotObj.translate.z = 0.0f;

  return teapotObj;
}

int main (void) {

  cr_exit_callback_init();
  cr_ctrl_init();
  cr_graphics_init();

  cr_camera* camera = cr_camera_create(75.0f, 0.5f, 1000.0f);

  ScePspFVector3 rot = { 0.0f, 0.0f, 0.0f };
  ScePspFVector3 mov = { 0.0f, 0.0f, 0.0f };

  ScePspFVector3 lightDir = {0.0f, 0.0f, 1.0f};

  //cr_graphics_mesh* mesh = util_obj_loadMesh("./res/objects/test.obj");
  cr_graphics_renderObj teapotObj = loadTeapot();
  int val = 0;

  while (!cr_exit_callback_closeRequested()) {


    cr_ctrl_update();

    mov.x = cr_ctrl_getAnalogX() * 0.2f;
    mov.z = cr_ctrl_getAnalogY() * 0.2f;

    cr_camera_movementAlongDirection(camera, mov);

    rot.y -= cr_ctrl_buttonPressed(CR_BUTTON_SQUARE) ? 0.03f : 0.0f;
    rot.y += cr_ctrl_buttonPressed(CR_BUTTON_CIRCLE) ? 0.03f : 0.0f;

    rot.x -= cr_ctrl_buttonPressed(CR_BUTTON_TRIANGLE) ? 0.03f : 0.0f;
    rot.x += cr_ctrl_buttonPressed(CR_BUTTON_CROSS) ? 0.03f : 0.0f;

    cr_camera_setRot(camera, &rot);
    cr_camera_useView(camera);

    cr_graphics_start();

    cr_graphics_clearScreen();

    cr_graphics_drawMesh(&teapotObj);

    cr_graphics_end();

    cr_graphics_present();

    teapotObj.rotate.x = val * 0.79f * (GU_PI/180.0f);
    teapotObj.rotate.y = val * 0.98f * (GU_PI/180.0f);
    teapotObj.rotate.z = val * 1.32f * (GU_PI/180.0f);

    val++;
  }

  sceKernelExitGame();

  return 0;
}
