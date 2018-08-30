#include <cmath>
#include "../utils/Color.h"
//Constructors

Color::Color(const Vector& colorV): colorV(colorV) {

    float x = sqrt(colorV.x);
    float y = sqrt(colorV.y);
    float z = sqrt(colorV.z);

    r = x * 255.99;
    g = y * 255.99;
    b = z * 255.99;
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
