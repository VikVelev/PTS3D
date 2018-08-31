#ifndef CAMERA_H
#define CAMERA_H

#ifndef RAY_H
#include "../main/Ray.cpp"
#endif

class Camera {
    public:
        Vector origin;
        Vector lowerLeftCorner;
        Vector horizontal;
        Vector vertical;
        
        Camera(float ratio) {
            lowerLeftCorner = Vector(-ratio , -ratio/2, -ratio/2);
            horizontal      = Vector(ratio*16/8,  0.0    ,  0.0    );
            vertical        = Vector(0.0    ,  ratio*9/8  ,  0.0    );
            origin          = Vector(0.0    ,  0.0    ,  0.0    );
        }

        Camera(
            const Vector& l, 
            const Vector& h, 
            const Vector& v, 
            const Vector& o
        ) {
            lowerLeftCorner = l;
            horizontal = h;
            vertical = v;
            origin = o;
        }

        Ray getRay(float u, float v) {
            return Ray(origin, lowerLeftCorner + u*horizontal + v*vertical - origin);
        }
};

#endif