#ifndef RAY_H
#define RAY_H
#include "Vector.h"

#ifndef NULL
#define NULL (void *)0
#endif

#ifndef PI
#define PI 3.1415926f
#endif

//'class'
struct Ray {
	Vector origin, direction;
    
    Ray();
    Ray(Vector v_o, Vector v_d) {
        origin = v_o;
        direction = v_d;
    }
    
    //TODO Implement color calculation;
    Color color(const Ray& r);
    inline Vector point_at_paramter(float t) const { return origin + t*direction; }
};

#endif //RAY_H