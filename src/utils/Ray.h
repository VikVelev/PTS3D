#ifndef RAY_H
#define RAY_H


#ifndef NULL
#define NULL (void *)0
#endif

#ifndef PI
#define PI 3.1415926f
#endif

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