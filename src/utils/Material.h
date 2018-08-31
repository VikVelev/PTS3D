#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>

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
#ifndef GEOMETRY_H                                              //
#include "Geometry.h"                                           //
#endif                                                          //
/*                           END                                */

class Material {
    public:
        virtual bool scatter(const Ray& rayIn, const hitRecord& record, Vector& attenuation, Ray& scattered) const = 0;
        virtual bool scatter() const = 0;
};

class Lambertian : public Material {
    public:
        Lambertian(const Vector& al) : albedo(al) {}
        virtual bool scatter(const Ray& rayIn, const hitRecord& record, Vector& attenuation, Ray& scattered) const {
            Vector target = record.p + record.normal + randomInUnitSphere();
            scattered = Ray(record.p, target - record.p);
            attenuation = albedo;
            
            return true;
        }

        virtual bool scatter() const {
            return true;
        };

        Vector albedo;
};

Vector reflect(const Vector& vector, const Vector& normal) {
    return vector - 2*dot(vector, normal)*normal;
}

class Metal : public Material {
    public:
        Metal(const Vector& al) : albedo(al) {}
        virtual bool scatter(const Ray& rayIn, const hitRecord& record, Vector& attenuation, Ray& scattered) const {

            Vector rayDir = rayIn.direction();
            rayDir.transformToUnit();

            Vector reflected = reflect(rayDir, record.normal);
            scattered = Ray(record.p, reflected);
            attenuation = albedo;
            
            return (dot(scattered.direction(), record.normal) > 0);
        }

        virtual bool scatter() const {
            return true;
        };

        Vector albedo;
};

Vector calculateColorVec(const Ray& ray, Hitable *world, int depth, bool normals = false) {

    hitRecord record;// t parameter of the hit Sphere

    if(world->hit(ray, 0.001, MAXFLOAT, record)) {
        Ray scattered;
        Vector attenuation;
        
        if (depth < 50 && record.matPtr->scatter(ray, record, attenuation, scattered)) {
            return attenuation*calculateColorVec(scattered, world, depth + 1);
        } else {
            return Vector(0, 0, 0);
        }

        // Vector target = record.p + record.normal + randomInUnitSphere();

        // if (normals) {
        //     return 0.5 * (Vector(record.normal.x, record.normal.y, record.normal.z) + 1); 
        //     //the same hack used below to transform t between 0 and 1
        // }

        // return 0.5 * calculateColorVec(Ray(record.p, target - record.p), world);

    } else {
        
        Vector unitDirection = ray.direction();
        unitDirection.transformToUnit(); //map between -1 and 1

        float t = 0.5 * (unitDirection.y + 1.0); //hack to get T between 0 and 1 so I can use it for colors

        //lerp
        return (1.0f - t) * Vector(1.0, 1.0, 1.0) +  t * Vector(0.5, 0.7, 1.0);
    }
}

#endif