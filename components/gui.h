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
#include "deserializer.h"

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

	draw_dynamic_box_with_text(display, window, gc, active_layout->id[i] , font , active_layout->anchor_x[i], active_layout->anchor_y[i], &active_layout->size_x[i], &active_layout->size_y[i],active_layout->label[i]);

	break;

      case TEXT:

	draw_dynamic_text(display, window, gc, active_layout->id[i] , font , active_layout->anchor_x[i], active_layout->anchor_y[i], &active_layout->size_x[i], &active_layout->size_y[i],active_layout->label[i]);

	break;

      case TEXT_UNDERLINED:
	
	draw_dynamic_underlined_text(display, window, gc, active_layout->id[i] , font , active_layout->anchor_x[i], active_layout->anchor_y[i], &active_layout->size_x[i], &active_layout->size_y[i],active_layout->label[i]);

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

  void execute_button_functionality(unsigned int callback_id) {
    // space to link your callback variables to functions of your code!
    // the callback values can then be linked to elements in you gui config file!

    switch(callback_id) {
    case 0 :
      
      std::cout << "no functionality linked :D" << std::endl;

      break;
      
    case 1 :
      
      std::cout << "red time!" << std::endl;

      XSetForeground(display, gc, 0xFF0000);

      draw_active_layout();
      
      break;
      
      
    }
    
    return;
    
  }

  void clip_cursor_position(unsigned int mouse_x, unsigned int mouse_y) {
    
    for(int i = 0; i < active_layout->id.size(); ++i) {
      
      if (
	 mouse_x > active_layout->anchor_x[i] &&
	 mouse_x < active_layout->size_x[i]+active_layout->anchor_x[i] &&
	 mouse_y > active_layout->anchor_y[i] &&
	 mouse_y < active_layout->size_y[i]+active_layout->anchor_y[i]
	 )
	{
	  std::cout << "clicked on button with id: " << active_layout->id[i] << " - and linked callback function id: " << active_layout->callback[i] << std::endl;

	  execute_button_functionality(active_layout->callback[i]);

	}      
      
    }

    return;

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

	draw_active_layout();

	XFlush(display);
	
      }

      if(event.type == MotionNotify) {	
	
	Window root;
	Window child;
	
	unsigned int psychiose;

	int mouse_x,mouse_y,win_x,win_y;
	
	XQueryPointer(display, window, &root, &child, &win_x, &win_y, &mouse_x, &mouse_y, &psychiose );
	
      }

      if (event.type == ButtonPress && event.xbutton.button == 1) { 

	printf("Left mouse button pressed at (%d, %d)\n", event.xbutton.x, event.xbutton.y);
	clip_cursor_position(event.xbutton.x,event.xbutton.y);
	
      } else if (event.type == ButtonRelease && event.xbutton.button == 1) {

	printf("Left mouse button released at (%d, %d)\n", event.xbutton.x, event.xbutton.y);

      }
                
      if (event.type == KeyPress) {
	
	char buffer[1];

	KeySym keysym;

	XLookupString(&event.xkey, buffer, sizeof(buffer), &keysym, NULL);
	printf("Key pressed: %c\n", buffer[0]);
	
	if(buffer[0] == 'x') {

	  std::cout << "shutting down!" << std::endl;
	  break;
	  
	}
	
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

    XSelectInput(display, window, ExposureMask | PointerMotionMask | ButtonReleaseMask | ButtonPressMask | KeyPressMask | StructureNotifyMask);

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

    set_gui_name("example window");
    
    std::cout << "Loading Layouts..." << std::endl;

    active_layout = new layout_struct;
    
    std::cout << "done... adding elements" << std::endl;

    //load a layout via file dynamically:
    active_layout = deserialize_layout_file("layouts/main_layout");

    //or statically declare your layout, in case you dont wanna change it!
    add_element(active_layout, BUTTON , 10, 70, 20 , 100, "i was statically declared!", 0);
    add_element(active_layout, BUTTON , 10, 100, 20 , 100, "me too :D - click me for red", 1);
      
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

