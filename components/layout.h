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
  
    virtual ~layout_element() = default;
    virtual void draw() const = 0; 

};


class element_button : public layout_element {
public:

  element_button(
		 
		 unsigned int ianchor_x,
		 unsigned int ianchor_y,
		 unsigned int isize_x,
		 unsigned int isize_y,
		 std::string ilabel,
		 unsigned int ibutton_id

		 ) : anchor_x(ianchor_x),
		     anchor_y(ianchor_y),
		     size_x(isize_x),
		     size_y(isize_y),
		     label(ilabel),
		     button_id(ibutton_id) {}

  void draw() const override {
    std::cout << "Drawing Button: " << label << std::endl;
    draw_box(display, )
  }

private:

  unsigned int anchor_x;
  unsigned int anchor_y;
  unsigned int size_x;
  unsigned int size_y;
  std::string label;
  unsigned int button_id;
      
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

 element_text_box(
		 unsigned int ianchor_x,
		 unsigned int ianchor_y,
		 unsigned int isize_x,
		 unsigned int isize_y,
		 std::string ilabel
		 ) : anchor_x(ianchor_x),
		     anchor_y(ianchor_y),
		     size_x(isize_x),
		     size_y(isize_y),
		     label(ilabel)  {}

  void draw() const override {

    std::cout << "Drawing TextBox: " << label << std::endl;

  }

private:

  unsigned int anchor_x;
  unsigned int anchor_y;
  unsigned int size_x;
  unsigned int size_y;
  std::string label;
  unsigned int button_id;
  
};


class layout {
public:

  Display *display;
  Window window;
  GC gc;

  
  void addElement(std::unique_ptr<layout_element> element) {

    elements.push_back(std::move(element));

  }
  
  void drawAll() const {

    for (const auto& element : elements) {

      element->draw();

    }

  }
  
private:
  std::vector<std::unique_ptr<layout_element>> elements; 

};

