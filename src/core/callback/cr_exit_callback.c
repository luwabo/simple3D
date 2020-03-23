#include "cr_exit_callback.h"

#include <pspkernel.h>

static bool request = false;

// call-back function that gets called once player wants to exit
// by using the playstation-symbol-button
static int exit_callback(int arg1, int arg2, void* common) {
  request = true;

  return 0;
}

// function that will be executed in a seperate thread (RR-scheduler)
// it will just create a callback and set the exit_callback function
static int callbackThread (SceSize args, void* argp) {
  int cbid;

  // creates the callback
  cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
  // sets it as the exit-callback
  sceKernelRegisterExitCallback(cbid);
  // just sleeps
  sceKernelSleepThreadCB();

  return 0;
}

// helper function that creates the callback-thread and than just sleeps
int cr_exit_callback_init(void) {

  int threadID = -1;

  // creates thread that sets callback and sleeps until ps-button is pressed
  threadID = sceKernelCreateThread("exit_callback_thread", callbackThread, 0x11, 0xfa0, 0, 0);

  if (threadID >= 0) {
    sceKernelStartThread(threadID, 0, 0);
  }

  return threadID;
}

bool cr_exit_callback_closeRequested (void) {
  return request;
}
