#ifndef RAY_H
#define RAY_H

#include "Vector.h"

struct Ray {
    Ray() {};
    Ray(const Vector& a, const Vector& b) {
        Origin = a;
        Direction = b;
    };

    Vector origin() const {
        return Origin;
    };

    Vector direction() const {
        return Direction;
    };

    Vector pointAtParameter(float t) const {
        return Origin + t * Direction; 
    };
    
    Vector Origin;
    Vector Direction;

    virtual ~Ray() {};
};

#endif //RAY_H