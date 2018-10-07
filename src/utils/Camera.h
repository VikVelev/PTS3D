#ifndef CAMERA_H
#define CAMERA_H

#ifndef RAY_H
#include "../main/Ray.cpp"
#endif

#include<stdio.h>
Vector randomInUnitDisk() {
    Vector p;
    do {
        p = 2 * Vector(drand48(), drand48(), 0) - Vector(1,1,0);
    } while (dot(p,p) >= 1.0);

    return p;
}

class Camera {
    public:
        Vector origin;
        Vector lowerLeftCorner;
        Vector horizontal;
        Vector vertical;
        Vector u, v, w;

        float lensRadius;

        Camera(Vector lookFrom, Vector lookAt, Vector vup, float vfov, float ratio, float aperature, float focusDistance) {
            //vfov = vertical fov;
            
            lensRadius = aperature/2;

            float theta = vfov * PI/180;
            float halfHeight = tan(theta/2);
            float halfWidth = ratio * halfHeight;

            origin = lookFrom;
            w = lookFrom - lookAt;
            w.transformToUnit();

            u = cross(vup, w);
            u.transformToUnit();

            v = cross(w, u);
            
            lowerLeftCorner = origin - halfWidth * focusDistance * u - halfHeight * focusDistance * v - focusDistance * w;
            horizontal = 2 * halfWidth * focusDistance * u;
            vertical = 2 * halfHeight * focusDistance * v;
        }

        Ray getRay(float u, float v) {
            Vector rd = lensRadius*randomInUnitDisk();
            Vector offset = u * rd.x + v * rd.y;

            return Ray(origin + offset, lowerLeftCorner + u*horizontal + v*vertical - origin - offset);
        }
};

#endif