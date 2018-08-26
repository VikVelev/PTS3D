#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#include "SDL2/SDL.h"
#include "Vector.cpp"
#include "Color.cpp"
#include "Ray.cpp"
#include "../utils/Geometry.h"

using namespace std;

static const int WIDTH = 1280;
static const int HEIGHT = 720;

static Color pixels[WIDTH][HEIGHT];


bool hitSphere(const Sphere& sphere, const Ray& ray){
    //The vector distance between the origin and the center
    Vector oc = ray.origin() - sphere.center;

    //Solving a simple quadratic equation
    float a = dot(ray.direction(), ray.direction());     //
    float b = 2.0 * dot(oc, ray.direction());            // All this, is the expanded vectorized formula for a sphere centered at the origin of a radius R;
    float c = dot(oc, oc) - sphere.radius*sphere.radius; //                                     X^2 + Y^2 + Z^2 = R^2
    float discriminant = b*b - 4*a*c;

    return (discriminant > 0);
}


Color calculateColor(const Ray& ray) {

    Sphere circle(  
        Vector(0.0f, 0.0f, -1.0f), //center
        0.5f, //radius
        Color(255, 0, 0) //color (red)
    );
    
    if(hitSphere(circle, ray)) {
        return circle.color;
    }

    Vector unitDirection = ray.direction();
    unitDirection.transformToUnit(); //map between 0 and 1 so I can use it for colors

    float t = 0.5 * (unitDirection.y + 1.0); //hack to get T between 0 and

    //using vectors to calculate the linear interpolation between colors, since I already have operator overloads for them
    Vector color = (1.0f - t) * Vector(0.5, 0.7, 1.0) +  t * Vector(1.0, 1.0, 1.0);

    int r = int(255.99*color.x);
    int g = int(255.99*color.y);
    int b = int(255.99*color.z);

    return Color(r, g, b);
}

int main() {

    printf( "Initializing...\n");
    // TODO implement Material class

    Vector lowerLeftCorner (-2.0, -1.0, -1.0);
    Vector horizontal      (4.0 ,  0.0,  0.0);
    Vector vertical        (0.0 ,  2.0,  0.0);
    Vector origin          (0.0 ,  0.0,  0.0);

    //Build Scene
    for (int j = 0; j < HEIGHT; j++) { // from right to left
        for (int i = 0; i < WIDTH; i++) {   // from up to down

            Vector2 gradient(
                float(i) / float(WIDTH), // u
                float(j) / float(HEIGHT)  // v
            );

            Ray r(origin, lowerLeftCorner + gradient.u*horizontal + gradient.v*vertical);

            Color color = calculateColor(r);

            pixels[i][j] = color;
        }
    }

    // SDL Window SETUP
    //The window we'll be rendering to
    SDL_Window* window = NULL;
    SDL_Event event;
    SDL_Renderer *renderer;

    //The surface contained by the window
    SDL_Surface* screenSurface = NULL;

    //Initialize SDL
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    } else {
        SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        if ( window == NULL ) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        } else {
            //Render scene
            for (int j = 0; j < HEIGHT; j++) {
                for (int i = 0; i < WIDTH; i++) {
                    SDL_SetRenderDrawColor(
                        renderer,
                        pixels[i][j].r,
                        pixels[i][j].g,
                        pixels[i][j].b,
                        255
                    );
                    SDL_RenderDrawPoint(renderer, i, j);
                }
            }

            SDL_RenderPresent(renderer);
            
            for (;;) {
                if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
                    printf("Exiting...\n");
                    break;
                }
            }

            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();

            return EXIT_SUCCESS;
        }
    }

    return 0;
}