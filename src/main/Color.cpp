#include <cmath>
#include "../utils/Color.h"
//Constructors

Color::Color(const Vector& colorV): colorV(colorV) {
    r = colorV.x * 255.99;
    g = colorV.y * 255.99;
    b = colorV.z * 255.99;
}

Color::Color(const Color& c): r(c.r), g(c.g), b(c.b) {

}

Color::Color(): r(255.0f), g(255.0f), b(255.0f) {

}

Color::Color(float r, float g, float b): r(r), g(g), b(b) {

}

//Destructor
Color::~Color() {

}
