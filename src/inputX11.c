#include "inputX11.h"
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>

Display *disp;
Window win;
int x11_fd;
fd_set in_fds;

struct timeval tv;
XEvent ev;

unsigned int kc;
int key = 0;

// DOWN 258  65364
// UP 259    65362
// LEFT 260  65361
// RIGHT 261 65363

int translate(int val) {
  switch (val) {
  case 65364:
    return 258;
  case 65362:
    return 259;
  case 65361:
    return 260;
  case 65363:
    return 261;
  case 113:
    return 133; // q
  case 98:
    return 98; // b
  case 118:
    return 118; // v
  default:
    return val;
  }
}

void initX11() {
  disp = XOpenDisplay(NULL);
  XAutoRepeatOff(disp);

  // You don't need all of these. Make the mask as you normally would.
  XGrabKeyboard(disp, DefaultRootWindow(disp), True, GrabModeAsync,
                GrabModeAsync, CurrentTime);

  // This returns the FD of the X11 display (or something like that)
  x11_fd = ConnectionNumber(disp);
}

int processX11Input() {
  // Create a File Description Set containing x11_fd
  FD_ZERO(&in_fds);
  FD_SET(x11_fd, &in_fds);

  // Set our timer.  One second sounds good.
  tv.tv_usec = 0;
  tv.tv_sec = 0;

  // Wait for X Event or a Timer
  int num_ready_fds = select(x11_fd + 1, &in_fds, NULL, NULL, &tv);
  if (num_ready_fds > 0) {
    // printf("Event Received!\n");
    XNextEvent(disp, &ev);
    switch (ev.type) {
    case KeyPress:
      kc = ((XKeyPressedEvent *)&ev)->keycode;
      key = XkbKeycodeToKeysym(disp, kc, 0, ev.xkey.state & ShiftMask ? 1 : 0);
      return translate(key);

    case KeyRelease:
      kc = ((XKeyPressedEvent *)&ev)->keycode;
      // key = XkbKeycodeToKeysym(disp, kc, 0, ev.xkey.state & ShiftMask ? 1 :
      // 0); return translate(key);
      return -999;

    case Expose:
      while (XCheckTypedEvent(disp, Expose, &ev)) /* empty body */
        ;
      break;
    case ButtonPress:
    case ButtonRelease:
    case MotionNotify:
    case ConfigureNotify:
    default:
      break;
    }
  }

  else if (num_ready_fds == 0) {
    // Handle timer here
    // printf("Timer Fired!\n");
  } else {
    // printf("An error occured!\n");
  }
  // Handle XEvents and flush the input
  while (XPending(disp))
    XNextEvent(disp, &ev);
  return 0;
}
