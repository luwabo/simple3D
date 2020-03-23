#ifndef CAMERA_H_
#define CAMERA_H_

#include <pspgu.h>

typedef struct cr_camera {

  ScePspFVector3 pos;
  ScePspFVector3 rot;

  float fov;

  float near;
  float far;

} cr_camera;

cr_camera* cr_camera_create (float fov, float near, float far);
void cr_camera_remove (cr_camera* camera);

void cr_camera_setPos (cr_camera* camera, ScePspFVector3* pos);
void cr_camera_setRot (cr_camera* camera, ScePspFVector3* rot);

ScePspFVector3 cr_camera_getPos (cr_camera* camera);
ScePspFVector3 cr_camera_getRot (cr_camera* camera);

void cr_camera_useView (cr_camera* camera);

// moving the camera relative to the direction it looks at. In other words:
// camera position is origin of new 3d-coordinate-system
void cr_camera_movementAlongDirection (cr_camera* camera, ScePspFVector3 mov);


#endif // CAMERA_H_
