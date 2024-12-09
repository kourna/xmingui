#pragma once

#define MARGIN_X 3
#define MARGIN_Y 3

#include <thread>
#include <X11/Xlib.h>
#include <unistd.h> 
#include <stdio.h>  
#include <iostream>
#include <cmath>
#include <chrono>
#include <vector>

void draw_box(Display* display, Window window, GC gc, unsigned int anchor_x, unsigned int anchor_y, unsigned int size_x, unsigned int size_y) {

  XDrawLine(display, window, gc, anchor_x, anchor_y, anchor_x+size_x, anchor_y);
  XDrawLine(display, window, gc, anchor_x+size_x, anchor_y, anchor_x+size_x, anchor_y+size_y);
  XDrawLine(display, window, gc, anchor_x+size_x, anchor_y+size_y, anchor_x, anchor_y+size_y);
  XDrawLine(display, window, gc, anchor_x, anchor_y+size_y, anchor_x, anchor_y);

  return;
}
