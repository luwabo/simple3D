#include "cr_camera.h"

#include <math.h>
#include <stdlib.h>

#include <pspgum.h>


cr_camera* cr_camera_create (float fov, float near, float far) {

  cr_camera* newCamera = (cr_camera*) malloc(sizeof(cr_camera));

  // new camera will look at the positive z-achsis
  newCamera->pos.x = 0.0f;
  newCamera->pos.y = 0.0f;
  newCamera->pos.z = 0.0f;

  newCamera->rot.x = 0.0f;
  newCamera->rot.y = 0.0f;
  newCamera->rot.z = 1.0f;

  newCamera->fov = fov;
  newCamera->near = near;
  newCamera->far = far;


  return newCamera;
}

void cr_camera_remove (cr_camera* camera) {

  free(camera);
}

void cr_camera_setPos (cr_camera* camera, ScePspFVector3* pos) {
  camera->pos.x = pos->x;
  camera->pos.y = pos->y;
  camera->pos.z = pos->z;
}
void cr_camera_setRot (cr_camera* camera, ScePspFVector3* rot) {
  camera->rot.x = rot->x;
  camera->rot.y = rot->y;
  camera->rot.z = rot->z;
}

ScePspFVector3 cr_camera_getPos (cr_camera* camera) {
  
  return camera->pos;
}

ScePspFVector3 cr_camera_getRot (cr_camera* camera) {

  return camera->rot;
}

void cr_camera_useView (cr_camera* camera) {

  sceGumMatrixMode(GU_PROJECTION);
  sceGumLoadIdentity();
  sceGumPerspective(camera->fov, 16.0f /  9.0f, camera->near, camera->far);

  sceGumMatrixMode(GU_VIEW);
  sceGumLoadIdentity();
  sceGumRotateXYZ(&camera->rot);
  sceGumTranslate(&camera->pos);
}

void cr_camera_movementAlongDirection (cr_camera* camera, ScePspFVector3 mov) {

  ScePspFMatrix4 viewMatrix;

  sceGumMatrixMode(GU_VIEW);
  sceGumStoreMatrix(&viewMatrix);

  // ------>
  // ------>
  // ------>
  // ------>
  camera->pos.x += viewMatrix.x.x * mov.x + viewMatrix.x.y * mov.y + viewMatrix.x.z * mov.z;
  camera->pos.y += viewMatrix.y.x * mov.x + viewMatrix.y.y * mov.y + viewMatrix.y.z * mov.z;
  camera->pos.z += viewMatrix.z.x * mov.x + viewMatrix.z.y * mov.y + viewMatrix.z.z * mov.z;
}
