#include <cmath>
#include "Color.h"

//Constructors


Color::Color(const Color& c): r(c.r), g(c.g), b(c.b) {

}

Color::Color(): r(255.0f), g(255.0f), b(255.0f) {

}

Color::Color(float r, float g, float b): r(r), g(g), b(b) {

}

//Destructor
Color::~Color() {

}
