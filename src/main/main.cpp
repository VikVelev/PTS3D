#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <omp.h>

#include "SDL2/SDL.h"
#include "Vector.cpp"
#include "Color.cpp"
#include "Ray.cpp"

#include "../utils/Geometry.h" //Important functions such as calculateColor
#include "../utils/Camera.h"
#include "../utils/Material.h"

using namespace std;

static const int WIDTH = 1280;
static const int HEIGHT = 720;
static const int SAMPLES = 200; //SPP

// SDL Window SETUP
//The window we'll be rendering to
SDL_Window* window = NULL;
SDL_Event event;
SDL_Renderer *renderer;

//The surface contained by the window
SDL_Surface* screenSurface = NULL;

Color pixels[WIDTH][HEIGHT]; // basically the screen
Vector pixelsColorStore[WIDTH][HEIGHT]; //for dynamic spp rendering

inline void renderScene() {
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            SDL_SetRenderDrawColor(
                renderer,
                (Uint8)pixels[i][(HEIGHT - 1) - j].r, // This is so we can flip the image horizontally
                (Uint8)pixels[i][(HEIGHT - 1) - j].g,
                (Uint8)pixels[i][(HEIGHT - 1) - j].b,
                255
            );
            //printf("%d %d \n", i, j);
            SDL_RenderDrawPoint(renderer, i, j);
        }
    }
    SDL_RenderPresent(renderer);
}

int main() {

    printf( "Initializing...\n");

    float ratio = float(WIDTH) / float(HEIGHT);
    //Build Scene

    Hitable *hitable[5];

    hitable[0] = new Sphere(  
        Vector(0, 0, -1), //center
        0.5f, //radius
        new Lambertian(Vector(0.5, 0.7, 1.0)) //color (blue)
    );

    hitable[1] = new Sphere(  
        Vector(0, -100.5f, -1), //center
        100.0f, //radius
        new Lambertian(Vector(0.8, 0.8, 0.0))
    );

    // Glass ball
    hitable[2] = new Sphere(  
        Vector(-1, 0, -1), //center
        -0.45f, //radius
        new Dielectric(1.5)
    );

    hitable[3] = new Sphere(  
        Vector(-1, 0, -1), //center
        0.5f, //radius
        new Dielectric(1.5)
    );
    //end glass ball

    hitable[4] = new Sphere(  
        Vector(1, 0, -1), //center
        0.5f, //radius
        new Metal(Vector(0.8, 0.6, 0.2), 0.2)        
    );
    // float R = cos(PI/4);
    // hitable[0] = new Sphere(Vector(-R,0,-1), R, new Lambertian(Vector(0,0,1)));
    // hitable[1] = new Sphere(Vector(R,0,-1), R, new Lambertian(Vector(1,0,0)));

    Hitable *world = new Scene(hitable, 5);

    Camera camera(
        Vector(-2, 2,  1),
        Vector( 0,  0, -1),
        Vector( 0,  1,  0),
        22, ratio);

    printf( "Calculating...\n");

    //Initialize SDL
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    } else {
        SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
    }

    time_t startTime;
    time(&startTime);

    for (int s = 0; s < SAMPLES; s++) {
        #pragma omp parallel for
        for (int j = 0; j < HEIGHT; j++) { // from right to left
            for (int i = 0; i < WIDTH; i++) {   // from up to down
                #pragma omp task shared(camera, world)
                {
                    if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
                        printf("Exiting...\n");

                        SDL_DestroyRenderer(renderer);
                        SDL_DestroyWindow(window);
                        SDL_Quit();
                    }

                    Ray r;
                    Color color;
                    
                    float u = float(i + drand48()) / float(WIDTH);
                    float v = float(j + drand48()) / float(HEIGHT);

                    r = camera.getRay(u, v);

                    pixelsColorStore[i][j] += calculateColorVec(r, world, 0);
                    color = Color((pixelsColorStore[i][j] / float(s + 1)));

                    pixels[i][j] = color;
                }
            }
        }

        renderScene();
    }

    printf("Done in %d seconds\n", time(0) - startTime);

    for(;;) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
            printf("Exiting...\n");

            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();

            break;
        }
    }
    return 0;
}
