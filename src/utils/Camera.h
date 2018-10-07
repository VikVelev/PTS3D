#ifndef CAMERA_H
#define CAMERA_H

#ifndef RAY_H
#include "../main/Ray.cpp"
#endif

#include<stdio.h>

class Camera {
    public:
        Vector origin;
        Vector lowerLeftCorner;
        Vector horizontal;
        Vector vertical;

        Camera(Vector lookFrom, Vector lookAt, Vector vup, float vfov, float ratio) {
            //vfov = vertical fov;
            Vector u, v, w;
            
            float theta = vfov * PI/180;
            float halfHeight = tan(theta/2);
            float halfWidth = ratio * halfHeight;

            origin = lookFrom;
            w = lookFrom - lookAt;
            w.transformToUnit();

            u = cross(vup, w);
            u.transformToUnit();

            v = cross(w, u);
            
            lowerLeftCorner = origin - halfWidth*u - halfHeight*v - w;
            horizontal = 2 * halfWidth * u;
            vertical = 2 * halfHeight * v;
        }

        Ray getRay(float u, float v) {
            return Ray(origin, lowerLeftCorner + u*horizontal + v*vertical - origin);
        }
};

#endif