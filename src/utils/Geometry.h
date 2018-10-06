#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <stdlib.h>

/* These are here because of linting issues. kur za cpp lintera */
#ifndef VECTOR_H                                                //
#include "../main/Vector.cpp"                                   //
#endif                                                          // 
                                                                //
#ifndef RAY_H                                                   //
#include "../main/Ray.cpp"                                      //
#endif                                                          //
                                                                //
#ifndef COLOR_H                                                 //
#include "../main/Color.cpp"                                    //
#endif                                                          //
                                                                //
/*                           END                                */

//Hitable abstraction, so we can use it as an interface later on.
class Material;

struct hitRecord {
    float t;
    Vector p;
    Vector normal;
    Material *matPtr;
};

class Hitable {
    public:
        virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const = 0;
};

class Scene: public Hitable {
    public:
        Hitable **list; //list of hitable objects
        int listSize;

        Scene() {}
        Scene(Hitable **l, int n) {
            list = l;
            listSize = n;
        }

        virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const;
};

bool Scene::hit(const Ray& ray, float tMin, float tMax, hitRecord& record) const {
    hitRecord tempRecord;
    bool hitAnything = false;
    double closestSoFar = tMax;

    for (int i = 0; i < listSize; i++) {
        if(list[i]->hit(ray, tMin, closestSoFar, tempRecord)) {
            hitAnything = true;
            closestSoFar = tempRecord.t;
            record = tempRecord;
        }
    }
    return hitAnything;
}

// Simple objects that are easy to work with:

class Sphere: public Hitable {
    public:
        Vector center;
        float radius;
        Material *mat;

        virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& record) const;

        Sphere() {};
        Sphere(const Vector& c, float r, Material *ma) {
            center = c;
            radius = r;
            mat = ma;
        };

        virtual ~Sphere() {};
};

inline bool Sphere::hit(const Ray& ray, float tMin, float tMax, hitRecord& record) const {
    //The vector distance between the origin and the center
    Vector oc = ray.origin() - center;
    record.matPtr = mat;

    //Solving a simple quadratic equation
    float a = dot(ray.direction(), ray.direction());     //
    float b = 2.0 * dot(oc, ray.direction());            // All this, is the expanded vectorized formula for a sphere centered at the origin of a radius R;
    float c = dot(oc, oc) - radius*radius; //                                     X^2 + Y^2 + Z^2 = R^2
    float discriminant = b*b - 4*a*c;

    if (discriminant > 0) {

        float root1 = (-b - sqrt(discriminant)) / (2*a);
        if (root1 < tMax && root1 > tMin) {
            record.t = root1;
            record.p = ray.pointAtParameter(root1);
            record.normal = (record.p - center) / radius;
            return true;
        }

        float root2 = (-b + sqrt(discriminant)) / (2*a);
        if (root2 < tMax && root2 > tMin) {
            record.t = root2;
            record.p = ray.pointAtParameter(root2);
            record.normal = (record.p - center) / radius;
            return true;
        }
    }

    return false;
}


// Other really important functions

Vector randomInUnitSphere() {
    Vector p;
    do {
        p = 2.0 * Vector(drand48(), drand48(), drand48()) - Vector(1, 1, 1);
    } while (p.length2() >= 1);

    return p;
}

#endif