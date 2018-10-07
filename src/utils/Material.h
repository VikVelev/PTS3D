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

static const int RAY_DEPTH = 50;

Vector reflect(const Vector& vector, const Vector& normal) {
    return vector - 2*dot(vector, normal)*normal;
}

bool refract(const Vector& vec, const Vector& n, float ni_over_nt, Vector& refracted) {
    Vector uv = transformToUnit(vec);
    float dotN = dot(uv, n);
    float discriminant = 1.0 - sqr(ni_over_nt)*(1-sqr(dotN));

    if(discriminant > 0) {
        refracted = ni_over_nt*(uv - n*dotN) - n*sqrt(discriminant);
        return true;
    } else {
        return false;
    }
}

float schlickApprox(float cosine, float refraction) {
    float r0 = (1 - refraction) / (1 + refraction);
    r0 = sqr(r0);
    return r0 + (1 - r0)*pow((1 - cosine), 5); 
}

class Material {
    public:
        virtual bool scatter(const Ray& rayIn, hitRecord& record, Vector& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
    public:
        Lambertian(const Vector& al) : albedo(al) {}
        virtual bool scatter(const Ray& rayIn, hitRecord& record, Vector& attenuation, Ray& scattered) const {
            
            Vector target = record.p + record.normal + randomInUnitSphere();
            scattered = Ray(record.p, target - record.p);
            attenuation = albedo;
            
            return true;
        }

        Vector albedo;
};

class Metal : public Material {
    public:
        Vector albedo;
        float fuzz;
        
        Metal(const Vector& al, float f) : albedo(al) {
            f < 1 ? fuzz = f : fuzz = 1;
        }

        virtual bool scatter(const Ray& rayIn, hitRecord& record, Vector& attenuation, Ray& scattered) const {

            Vector rayDir = rayIn.direction();
            rayDir.transformToUnit();

            Vector reflected = reflect(rayDir, record.normal);
            scattered = Ray(record.p, reflected + fuzz*randomInUnitSphere());
            attenuation = albedo;
            
            return (dot(scattered.direction(), record.normal) > 0);
        }
};

class Dielectric : public Material {
    public:
        float refraction;

        Dielectric(float r) : refraction(r) {}

        virtual bool scatter(const Ray& rayIn, hitRecord& record, Vector& attenuation, Ray& scattered) const {
            
            attenuation = Vector(1.0, 1.0, 1.0); //Basically what colors does the dielectric material absorb (0,0,0) means all

            Vector outward_normal;
            Vector reflected = reflect(rayIn.direction(), record.normal);
            
            float ni_over_nt;
            Vector refracted;

            float reflect_probability;
            float cosine;

            if(dot(rayIn.direction(), record.normal) > 0) {
                outward_normal = -record.normal;
                ni_over_nt = refraction;
                cosine = refraction * dot(rayIn.direction(), record.normal) / rayIn.direction().length();
            } else {
                outward_normal = record.normal;
                ni_over_nt = 1.0 / refraction;
                cosine = -dot(rayIn.direction(), record.normal) / rayIn.direction().length();
            }

            if (refract(rayIn.direction(), outward_normal, ni_over_nt, refracted)) {
                reflect_probability = schlickApprox(cosine, refraction);
            } else {
                reflect_probability = 1.0;
            }

            if(drand48() < reflect_probability) {
                scattered = Ray(record.p, reflected);
            } else {
                scattered = Ray(record.p, refracted);
            }

            return true;
        }
};

Vector calculateColorVec(const Ray& ray, Hitable *world, int depth, bool normals = false) {

    hitRecord record;// t parameter of the hit Sphere

    if(world->hit(ray, 0.001, MAXFLOAT, record)) {

        Ray scattered;
        Vector attenuation;


        // TODO*: This is why the above todo happens
        if (depth < RAY_DEPTH && record.matPtr->scatter(ray, record, attenuation, scattered)) {
            return attenuation * calculateColorVec(scattered, world, depth + 1);
        } else {
            return Vector(0, 0, 0);
        }

    } else {
        
        Vector unitDirection = ray.direction();
        unitDirection.transformToUnit(); //map between -1 and 1

        float t = 0.5 * (unitDirection.y + 1.0); //hack to get T between 0 and 1 so I can use it for colors

        //lerp
        return (1.0f - t) * Vector(1.0, 1.0, 1.0) +  t * Vector(0.5, 0.7, 1.0);
    }
}

#endif