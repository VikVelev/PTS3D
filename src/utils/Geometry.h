#ifndef GEOMETRY_H
#define GEOMETRY_H

#ifndef VECTOR_H
#include "../main/Vector.cpp"
#endif

#ifndef COLOR_H
#include "../main/Color.cpp"
#endif

struct Sphere {
    Vector center;
    float radius;
    Color color;

    Sphere() {};
    Sphere(const Vector& c, float r, const Color& cl) {
        center = c;
        radius = r;
        color = cl;
    };

    virtual ~Sphere() {};
};


#endif