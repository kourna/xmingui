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

  element_button(const std::string& label) : label(label) {}

  void draw() const override {
    std::cout << "Drawing Button: " << label << std::endl;

  }

private:
  std::string label;

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

  element_text_box(const std::string& placeholder) : placeholder(placeholder) {}

  void draw() const override {

    std::cout << "Drawing TextBox: " << placeholder << std::endl;

  }

private:
  std::string placeholder;

};


class layout {
public:

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

