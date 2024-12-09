#pragma once

#include <variant>
#include <thread>
#include <X11/Xlib.h>
#include <unistd.h> 
#include <stdio.h>  
#include <iostream>
#include <cmath>
#include <chrono>
#include <vector>
#include <memory>

#include "drawutils.h"

enum valid_element_types {

  BUTTON,
  TABLE,
  TEXT_INPUT_BOX,
  TEXT_BOX,
  BUTTON_LABELED,
  IMAGE,
  SLIDER,
  SLIDER_LABELED,
  BORDER

};

class layout_element {
public:

  Display* display;
  Window window;
  GC gc;
  
  virtual ~layout_element() = default;
  virtual void draw() const = 0; 
  
};


class element_button : public layout_element {
public:

  Display* display;
  Window window;
  GC gc;
  unsigned int anchor_x;
  unsigned int anchor_y;
  unsigned int size_x;
  unsigned int size_y;
  std::string label;
  unsigned int button_id;
   
  element_button(
		 Display* idisplay,
		 Window iwindow,
		 GC igc,
		 unsigned int ianchor_x,
		 unsigned int ianchor_y,
		 unsigned int isize_x,
		 unsigned int isize_y,
		 std::string ilabel,
		 unsigned int ibutton_id

		 ) :

    display(idisplay),
    window(iwindow),
    gc(igc),
    anchor_x(ianchor_x),
    anchor_y(ianchor_y),
    size_x(isize_x),
    size_y(isize_y),
    label(ilabel),
    button_id(ibutton_id) {}

  void draw() const override {

    std::cout << "Drawing Button: " << label << std::endl;

    //    draw_box(display,window,gc,10,10,10,10);

  }
     
};


class element_label : public layout_element {
public:

  element_label(const std::string& text) : text(text) {}

  void draw() const override {
    std::cout << "Drawing Label: " << text << std::endl;

  }

private:
  std::string text;

};


class element_text_box : public layout_element {
public:
  
  Display* display;
  Window window;
  GC gc;
  unsigned int anchor_x;
  unsigned int anchor_y;
  unsigned int size_x;
  unsigned int size_y;
  std::string label;
   
 element_text_box(

		  Display* idisplay,
		  Window iwindow,
		  GC igc,
		  unsigned int ianchor_x,
		  unsigned int ianchor_y,
		  unsigned int isize_x,
		  unsigned int isize_y,
		  std::string ilabel
		  
		  ) :
   
   display(idisplay),
   window(iwindow),
   gc(igc),
   anchor_x(ianchor_x),
   anchor_y(ianchor_y),
   size_x(isize_x),
   size_y(isize_y),
   label(ilabel)  {}
  
  void draw() const override {

    std::cout << "Drawing TextBox: " << label << std::endl;

  }

};


class layout {
public:
  
  std::vector<std::unique_ptr<layout_element>> elements; 
  
  void addElement(std::unique_ptr<layout_element> element) {

    std::cout << "adding element" << std::endl;

    elements.push_back(std::move(element));

  }
  
  void drawAll(Display* display) const {

    std::cout << "tryna draw shit" << std::endl;
    
    for (const auto& element : elements) {

      std::cout << "tryna draw element" << std::endl;
      
      element->draw();

    }

    XFlush(display);
    
  }
  
};

