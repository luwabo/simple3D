#ifndef CR_CTRL_H_
#define CR_CTRL_H_

#include <stdbool.h>

#include <pspctrl.h>

#define CR_BUTTON_SQUARE PSP_CTRL_SQUARE
#define CR_BUTTON_TRIANGLE PSP_CTRL_TRIANGLE
#define CR_BUTTON_CIRCLE PSP_CTRL_CIRCLE
#define CR_BUTTON_CROSS PSP_CTRL_CROSS

#define CR_BUTTON_UP PSP_CTRL_UP
#define CR_BUTTON_DOWN PSP_CTRL_DOWN
#define CR_BUTTON_LEFT PSP_CTRL_LEFT
#define CR_BUTTON_RIGHT PSP_CTRL_RIGHT

#define CR_BUTTON_START PSP_CTRL_START
#define CR_BUTTON_SELECT PSP_CTRL_SELECT

#define CR_BUTTON_LTRIGGER PSP_CTRL_LTRIGGER
#define CR_BUTTON_RTRIGGER PSP_CTRL_RTRIGGER

// TODO: add some features that xlab has!!

// init control functionality
void cr_ctrl_init (void);

// pulls and processes data to be used in API
void cr_ctrl_update (void);

// gives back a bool wether or not the button is pressed
bool cr_ctrl_buttonPressed (int button);

// gives back a bool wether or not the button was tapped
bool cr_ctrl_buttonTapped (int button);

// gives back analog-stick values from -1 to 1
float cr_ctrl_getAnalogX (void);
float cr_ctrl_getAnalogY (void);

#endif // CR_CTRL_H_
