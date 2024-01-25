#include "../utils/Color.h"
#include <cmath>
// Constructors

Color::Color(const Vector &colorV) : colorV(colorV) {
  // gamma correction
  float x = sqrt(colorV.x);
  float y = sqrt(colorV.y);
  float z = sqrt(colorV.z);

  // float x = colorV.x;
  // float y = colorV.y;
  // float z = colorV.z;

  r = x * 255.99;
  g = y * 255.99;
  b = z * 255.99;
}

Color::Color(const Color &c) : r(c.r), g(c.g), b(c.b) {}

Color::Color() : r(0.0f), g(0.0f), b(0.0f) {}

Color::Color(float r, float g, float b)
    : r(r), g(g), b(b) {}

// Destructor
Color::~Color() {}
