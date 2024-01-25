#ifndef COLOR_H
#define COLOR_H

#include "Vector.h"

struct Color {
  float r, g, b;
  Vector colorV;
  Color();
  Color(const Color &c);
  Color(const Vector &colorV);
  Color(float r, float g, float b);
  Color(float color);

  virtual ~Color();
};

#endif