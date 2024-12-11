#pragma once

#include <thread>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h> 
#include <stdio.h>  
#include <iostream>
#include <cmath>
#include <chrono>
#include <vector>

#include "layout.h"

class x_gui {

public:

  Display* display;
  Window window;
  GC  gc;

  bool shutdown = false;

  XFontStruct* font;

  layout_struct *active_layout;

  void draw_active_layout() {

    for(unsigned int i = 0; i < active_layout->id.size(); ++i) {

      switch(active_layout->type[i]) {

      case BUTTON:

	draw_dynamic_box_with_text(display, window, gc, active_layout->id[i] , font , active_layout->anchor_x[i], active_layout->anchor_y[i], active_layout->size_x[i],active_layout->size_y[i],active_layout->label[i]);

	break;
      case TEXT_BOX:

	draw_box(display, window, gc, active_layout->anchor_x[i], active_layout->anchor_y[i], active_layout->size_x[i], active_layout->size_y[i]);

	break;
      case BORDER:

	draw_dynamic_window_border(display,window,gc,active_layout->anchor_x[i]);

	break;

      }

      XFlush(display);

    }

  }
  
  void set_gui_name(std::string name) {

    XStoreName(display, window, name.c_str());

    return;

  }
  
  void window_runtime_helper(XFontStruct* font) {
    
    std::cout << __func__ << " loading fonts..." << std::endl;
    
    XEvent event;
    int cur_line = 10;
    
    XSetFont(display, gc, font->fid);
    
    while(!shutdown) {
      
      XNextEvent(display, &event);
      
      if (event.type == Expose | event.type == ConfigureNotify ) {

	std::cout << "Window needs to be redrawn.." << std::endl;       

	//draw all components

	XFlush(display);
	
      }

      if(event.type == MotionNotify) {	
	
	Window root;
	Window child;
	
	unsigned int psychiose;

	int mouse_x,mouse_y,win_x,win_y;
	
	std::cout << "calling xqery" << std::endl;
	XQueryPointer(display, window, &root, &child, &win_x, &win_y, &mouse_x, &mouse_y, &psychiose );
	
      }

      if (event.type == ButtonPress && event.xbutton.button == 1) { 

	printf("Left mouse button pressed at (%d, %d)\n", event.xbutton.x, event.xbutton.y);

      } else if (event.type == ButtonRelease && event.xbutton.button == 1) {

	printf("Left mouse button released at (%d, %d)\n", event.xbutton.x, event.xbutton.y);

      }
                
      if (event.type == KeyPress) {
	
	char buffer[1];

	KeySym keysym;

	printf("before loopup");
	
	XLookupString(&event.xkey, buffer, sizeof(buffer), &keysym, NULL);
	printf("Key pressed: %c\n", buffer[0]);

	printf("after loopup");
	
	if(buffer[0] == 'x')
	  break;
	
	if(buffer[0] == 'w') {
	  
	  //  active_layout->add_element(BUTTON,10,cur_line,100,10, "Crazy Button");

	  //  cur_line=cur_line+active_layout->layout->size_y[1];
	  

	}
	
	if(buffer[0] == 's') {
	  
	  // clip_cursor_position(mouse_x,mouse_y);


	}

	// draw_window(active_layout->get_window_layout());
	
      }
      
    }
    
    return;

  } 

  int init_gui(void) {
    
    std::cout << "Trying to allocate GUI." << std::endl;
      
    display = XOpenDisplay(NULL);

    if (!display)
      {
        fprintf(stderr, "error:"
                "       Can't open connection to display server."
                " Probably X server is not started.\n");
        return 1;
      }

    int blackColor = BlackPixel(display, DefaultScreen(display));
    int whiteColor = WhitePixel(display, DefaultScreen(display));

    window = XCreateSimpleWindow(
                                 display,
                                 DefaultRootWindow(display),// parent window
                                 0,//x
                                 0,//y
                                 720,//width
                                 480,//height
                                 2, //border width
                                 whiteColor,//border color
                                 whiteColor); // background color

    XSelectInput(display, window, ExposureMask | PointerMotionMask | KeyPressMask | StructureNotifyMask);

    font = XLoadQueryFont(display, "fixed");
    if (!font) {
      std::cerr << "Unable to load font" << std::endl;
      return 1;
    }
    
    XMapWindow(display, window);

    gc = XCreateGC(display, window, 0, NULL); // graphic context for render?

    set_gui_name("placeholder");
    
    XSetForeground(display, gc, blackColor);

    for(;;) {
      XEvent e;
      XNextEvent(display, &e);
      if (e.type == MapNotify)
        break;
    }

    std::cout << "Window allocation successful!" << std::endl;
    
    //================================= MAIN LAYOUT CONFIG SPACE ================================= 

    std::cout << "Loading Layouts..." << std::endl;

    active_layout = new layout_struct;

    if(!active_layout){std::cout << "error creating new layout!" << std::endl;}
    
    std::cout << "done... adding elements" << std::endl;

    add_element(active_layout, BUTTON , 10, 10, 20 , 100, "hi", 0);

    std::cout << "done! " << std::endl;

    draw_active_layout();
    
    //================================= WINDOW HANDLER =================================
    
    std::cout << "Launching window runtime helper..." << std::endl;

    window_runtime_helper(font);

    //Cleanup shoutout to feroxtard
    XUnloadFont(display, font->fid);
    XUnmapWindow(display, window);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;

  }

};

