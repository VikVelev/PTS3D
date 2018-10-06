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

static const int WIDTH = 400;
static const int HEIGHT = 200;
static const int SAMPLES = 100; //SPP

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

    Hitable *hitable[4];

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

    hitable[2] = new Sphere(  
        Vector(-1.1, 0, -1), //center
        0.5, //radius
        new Metal(Vector(0.8, 0.6, 0.2), 0.8)
    );

    hitable[3] = new Sphere(  
        Vector(1.1, 0, -1), //center
        0.5, //radius
        new Dielectric(2)
    );

    Hitable *world = new Scene(hitable, 4);
    Camera camera(ratio);

    printf( "Calculating...\n");

    //Initialize SDL
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    } else {
        SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
    }

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

    renderScene();
    printf("Done\n");

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
