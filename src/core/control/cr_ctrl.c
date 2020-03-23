#include "cr_ctrl.h"


#define BUTTON_PRESSED(buttonVal, buttons) (((buttonVal) & (buttons)) != 0)

static int buttons_crnt;
static int buttons_last;

static float analog_x;
static float analog_y;

void cr_ctrl_init (void) {
  sceCtrlSetSamplingCycle(0);
  sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
}

void cr_ctrl_update (void) {

  SceCtrlData pad_read;

  sceCtrlReadBufferPositive(&pad_read, 1);

  buttons_last = buttons_crnt;
  buttons_crnt = pad_read.Buttons;

  analog_x = -(pad_read.Lx - 127.5f) / 127.5f;
  analog_y = -(pad_read.Ly - 127.5f) / 127.5f;
}

bool cr_ctrl_buttonTapped (int button) {
  return !BUTTON_PRESSED(button, buttons_crnt) && BUTTON_PRESSED(button, buttons_last);
}

bool cr_ctrl_buttonPressed (int button) {
  return BUTTON_PRESSED(button, buttons_crnt);
}

float cr_ctrl_getAnalogX (void) {
  return analog_x;
}
float cr_ctrl_getAnalogY (void) {
  return analog_y;
}
