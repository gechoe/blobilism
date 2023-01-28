/**
 * blobilism.cpp
 * Author: Grace Choe
 * Date: 1/26/2022
 * 
 * Description:
 * This program implements a drawing program that supports a circle brush type, 
 * multiple brush sizes (no smaller than size 1, brush's size changes by clicking
 * the down and up arrow keys), multiple colors with a palette (seven colors),
 * transparency (no smaller than 0, transparency changed by the left and right
 * arrow keys), and clearing the canvas/all images (clearing done by the 'c' key).
 * 
 * The program's goal is to build and write a simple interactive application that
 * responds to user key and mouse events. It uses std::vector and works within
 * a 2D coordinate system.
 * 
 * Small note: Alpha variable made into a double and then converted to a float
 * when used within color() as alpha in float form leads to precision errors.
 * (Eg. 0.1 - 0.05 = 0.0499998) 
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include "tinygl-cpp.h"
using namespace tinygl;
using namespace std;

// rgb struct
// Struct for holding the red, green, blue color values
struct rgb {
  float red, green, blue;
  // alpha made into double to fix precision error when printing out alpha's
  // change in size
  double alpha;
};

// drawCircle class
// class for making a circle for the user to draw
class drawCircle {
  public:
    int size;
    rgb rgbColors;
    float xCoord, yCoord;
};

// palette class 
// class for making palette
class palette {
  public:
    rgb rgbC;
    float xLoc, yLoc;
};

// MyWindow class
// class for designating the ongoings within the program
class MyWindow : public Window {
  public:
    MyWindow(int w, int h) : Window(w, h) {
      circles.size = currSize;
      circles.rgbColors = currColor;
      circles.rgbColors.alpha = alpha;

      // Setting up the palette colors and pushing onto myPal vector to draw its
      // elements in draw()
      int startX = 35;
      for (rgb mc : myColors) {
        palette newPal;
        newPal.rgbC = mc;
        newPal.xLoc = startX;
        newPal.yLoc = 35;
        myPal.push_back(newPal);
        startX += 60;
      }
    }

    void setup() override {
      std::cout << "Window size: " << width() << ", " << height() << std::endl;
    }

    // mouseMotion()
    // Pushes a circle onto the vector/list to draw the circle later based on the
    // mouse location informaion stored
    virtual void mouseMotion(int x, int y, int dx, int dy) override {
      if (mouseIsDown(GLFW_MOUSE_BUTTON_LEFT)) {
        // todo: store a circle with the current color, size, x, y
        if (y > 70) { // Prevents user from drawing underneath palette
          drawCircle newCircle = circles;
          newCircle.xCoord = x;
          newCircle.yCoord = y;
          circleVector.push_back(newCircle);
        }
      }
    }

    // mouseDown()
    // Changes brush color based on if color within the color palette was clicked
    virtual void mouseDown(int button, int mods) override {
      if (button == GLFW_MOUSE_BUTTON_LEFT) {
        // todo: check if user clicked a color
        float mx = mouseX();  // current mouse pos x
        float my = mouseY();  // current mouse pos y
        // Calculations to find eraser's rectangular boundaries/limits
        float eraserWLimit = eraserWidth / 2, eraserYLimit = eraserHeight / 2;
        std::cout << std::setprecision(6) << std::fixed;

        // if the user clicks the eraser
        if ((mx > eraserX - eraserWLimit) && (mx < eraserX + eraserWLimit) &&
          (my > eraserY - eraserYLimit) && (my < eraserY + eraserYLimit)) {
          circles.rgbColors = rgb{0.95f, 0.95f, 0.95f, circles.rgbColors.alpha};
          std::cout << "Setting color to " << circles.rgbColors.red <<  " "
            << circles.rgbColors.green << " " << circles.rgbColors.blue << "\n";
        }

        // if user clicks a color from the palette colors
        for (palette pal : myPal) {
          float dx = mx - pal.xLoc;
          float dy = my - pal.yLoc;
          float dist = sqrt((dx * dx) + (dy * dy));
          
          // Checks if user clicked a palette color by seeing if user's mouse
          // position is within the 25 radius from the center of the color's
          // circle
          if (dist < 25) {
            circles.rgbColors = rgb{pal.rgbC.red, pal.rgbC.green, pal.rgbC.blue,
              circles.rgbColors.alpha};
            std::cout << "Setting color to " << circles.rgbColors.red <<  " "
              << circles.rgbColors.green << " " << circles.rgbColors.blue << "\n";
          }
        }
      }
    }

    // keyDown()
    // Changes brush size, brush transparency, and clears screen if certain keys
    // are pressed
    void keyDown(int key, int mods) override {
      if (key == GLFW_KEY_UP) { // Up arrow key
        std::cout << std::setprecision(0) << std::fixed;
        // Increase size of circle
        if (circles.size == 1) {
          circles.size += 4;
        } else {
          circles.size += 5;
        }
        std::cout << "Pressed UP: Increase point size to " << circles.size << "\n";
      } else if (key == GLFW_KEY_DOWN) { // Down arrow key
        std::cout << std::setprecision(0) << std::fixed;
        // Decrease size of circle
        if (circles.size > 5) {
          circles.size -= 5;
          std::cout << "Pressed DOWN: Decrease point size to " << circles.size
            << "\n";
        } else if (circles.size == 5) {
          circles.size -= 4;
          std::cout << "Pressed DOWN: Decrease point size to " << circles.size
            << "\n";
        } else {
          std::cout << "Pressed DOWN: Cannot decrease size, at limit "
            << circles.size << "\n";
        }
      } else if (key == GLFW_KEY_LEFT) { // Left arrow key
        std::cout << std::setprecision(2) << std::fixed;
        // Decrease alpha, makes more trasnparent
        circles.rgbColors.alpha -= 0.05;

        if (circles.rgbColors.alpha >= 0) {
          std::cout << "Pressed LEFT: Decrease transparency to "
            << circles.rgbColors.alpha << "\n";
        } else {
          circles.rgbColors.alpha = 0;
          std::cout << "Pressed LEFT: Cannot decrease transparency, at limit "
            << circles.rgbColors.alpha << "\n";
        }
      } else if (key == GLFW_KEY_RIGHT) { // Right arrow key
        std::cout << std::setprecision(2) << std::fixed;
        // Increase alpha
        if (circles.rgbColors.alpha < 1) {
          circles.rgbColors.alpha += 0.05;
          std::cout << "Pressed RIGHT: Increase transparency to "
            << circles.rgbColors.alpha << "\n";
        } else {
          std::cout << "Pressed RIGHT: Cannot increase transparency, at limit "
            << circles.rgbColors.alpha << "\n";
        }
      } else if (key == GLFW_KEY_C) { // 'C' key
        // Clear vector of circles
        circleVector.clear();
      }
    }

    // draw()
    // Draws the screen's elements (color palette) and the user's drawn image by
    // drawing all the circles saved within the circles vector
    void draw() override {
      background(0.95f, 0.95f, 0.95f); // parameters: r, g, b

      // Commented out, showed a pink circle to see adjustment in pen size
      // color(1.0f, 0.5f, 0.5f);
      // circle(width() * 0.5f, height() * 0.5, circles.size);

      // Drawing what user draws
      // Done first before drawing the color palette circles to prevent user from
      // drawing atop the palette
      for (drawCircle c : circleVector) {
        color(c.rgbColors.red, c.rgbColors.green, c.rgbColors.blue, float(c.rgbColors.alpha));
        circle(c.xCoord, c.yCoord, c.size);
      }

      // todo : draw palette
      // Palette black background
      color(0.1f, 0.1f, 0.1f);
      square(width()/2.0f, 35, width(), 70);

      // Drawing color palette circles
      for (palette p : myPal) {
        color(p.rgbC.red, p.rgbC.green, p.rgbC.blue);
        circle(p.xLoc, p.yLoc, 50);
      }

      // Drawing eraser
      // Pink half of eraser
      color(1.0f, 0.7f, 0.7f);
      square(eraserX, eraserY, eraserWidth, eraserHeight);
      // Blue half of eraser
      color(0.3f, 0.5f, 1.0f);
      square(eraserX, eraserY - (eraserHeight / 4), eraserWidth, eraserHeight / 2);
      // White connector of eraser
      color(0.9f, 0.9f, 0.9f);
      square(eraserX, eraserY, eraserWidth, eraserHeight / 10);
    }
    
  private:
    // todo: create member variables for:
    // Drawing vector
    drawCircle circles;
    // Current circle size
    float currSize = 10;
    // Current transparency, made into double to fix precision error with floats
    double alpha = 1.0;
    // Current color
    rgb currColor = rgb{0.95f, 0.95f, 0.95f};
    // List of circles to draw each frame
    vector<drawCircle> circleVector;
    // Color palette
    vector<palette> myPal;
    // Colors for palette in myColors vector: dark blue, medium blue, light blue,
    // light yellow, magenta, dark green, brown
    vector<rgb> myColors = {{0.0f, 0.19921875f, 0.3984375f}, {0.0f, 0.5f, 1.0f},
      {0.59765625f, 0.796875f, 1.0f}, {0.99609375f, 0.9375f, 0.6796875f},
      {0.59765625f, 0.3984375f, 0.69921875f}, {0.296875f, 0.5f, 0.3984375f},
      {0.3984375f, 0.296875f, 0.19921875f}};
    // Eraser's values
    float eraserX = width() - 40, eraserY = 35, eraserWidth = 35, eraserHeight = 45;
};

// main()
int main() {
  MyWindow window(500, 500);
  window.run();
}
