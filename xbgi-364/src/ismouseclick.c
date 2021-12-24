#include "graphics.h"

/* by GG */

#include <stdio.h>

int ismouseclick (int kind)
{
  XFlush (dpy);
  
  while (1) {
    XEvent event;
    if (XCheckMaskEvent (dpy, ButtonPressMask, &event)) { // non blocking
      if (event.type == ButtonPress) {
	mouse_x = event.xkey.x;
	mouse_y = event.xkey.y;
	button = event.xbutton.button;
	return (button == kind);
      }
      else 
	return 0;
    }
    else
      return 0;
  }
}
