#include <iostream>
#include <vector>
#include "tinygl-cpp.h"
using namespace tinygl;

class MyWindow : public Window {
 public:
  MyWindow(int w, int h) : Window(w, h) {}

  void setup() override {
    std::cout << "Window size: " << width() << ", " << height() << std::endl;
  }

  virtual void mouseMotion(int x, int y, int dx, int dy) {
    if (mouseIsDown(GLFW_MOUSE_BUTTON_LEFT)) {
      // todo: store a circle with the current color, size, x, y
    }
  }
  
  virtual void mouseDown(int button, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
      // todo: check if user clicked a color 
      // float mx = mouseX();  // current mouse pos x
      // float my = mouseY();  // current mouse pos y
    }
  }

  void keyDown(int key, int mods) {
    if (key == GLFW_KEY_UP) {
      // increase size of circle
    } 
    else if (key == GLFW_KEY_DOWN) {
      // decrease size of circle
    }
    else if (key == GLFW_KEY_LEFT) {
      // decrease alpha
    }
    else if (key == GLFW_KEY_RIGHT) {
      // increase alpha
    }
    else if (key == GLFW_KEY_C) {
      // clear vector of circles
    }
  }

  void draw() override {
    background(0.95f, 0.95f, 0.95f); // parameters: r, g, b

    // todo : draw pallet
    color(0.1f, 0.1f, 0.1f);
    square(width()/2.0f, 35, width(), 70);
  }
 private:

  // todo: create member variables for 
  // current circle size
  // current transparency
  // current color
  // list of circles to draw each frame
  // color pallet
};

int main() {
  MyWindow window(500, 500);
  window.run();
}