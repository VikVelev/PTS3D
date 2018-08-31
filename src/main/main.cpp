#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#include "SDL2/SDL.h"
#include "Vector.cpp"
#include "Color.cpp"
#include "Ray.cpp"

#include "../utils/Geometry.h" //Important functions such as calculateColor
#include "../utils/Camera.h"
#include "../utils/Material.h"

using namespace std;

static const int WIDTH = 1200;
static const int HEIGHT = 600;
static const int SAMPLES = 6; //ANTIALIASING SAMPLES set to 0 to turn of antialiasing

static Color pixels[WIDTH][HEIGHT];

int main() {

    printf( "Initializing...\n");
    // TODO implement Material class

    float ratio = float(WIDTH) / float(HEIGHT);
    //Build Scene

    Hitable *hitable[4];

    hitable[0] = new Sphere(  
        Vector(0, 0, -1), //center
        0.5f, //radius
        new Lambertian(Vector(0.8, 0.3, 0.3)) //color (red)
    );

    hitable[1] = new Sphere(  
        Vector(0, -100.5f, -1), //center
        100.0f, //radius
        new Lambertian(Vector(0.8, 0.8, 0.0))
    );

    hitable[2] = new Sphere(  
        Vector(-1.1, 0, -1), //center
        0.5, //radius
        new Metal(Vector(0.8, 0.6, 0.2))
    );

    hitable[3] = new Sphere(  
        Vector(1.1, 0, -1), //center
        0.5, //radius
        new Metal(Vector(0.8, 0.8, 0.8))
    );

    Hitable *world = new Scene(hitable, 4);
    Camera camera(ratio);

    for (int j = 0; j < HEIGHT; j++) { // from right to left
        for (int i = 0; i < WIDTH; i++) {   // from up to down

            float u, v;
            Ray r;
            Color color;
            
            //ANTIALIASING

            if (SAMPLES > 0) {

                Vector colorStore(0, 0, 0);

                for (int s = 0; s < SAMPLES; s++) {
                    u = float(i + drand48()) / float(WIDTH);
                    v = float(j + drand48()) / float(HEIGHT);
                    
                    r = camera.getRay(u, v);
                    Vector p = r.pointAtParameter(2.0);
                    colorStore += calculateColorVec(r, world, 0);
                }

                colorStore /= float(SAMPLES);
                color = Color(colorStore);

            } else {

                u = float(i / float(WIDTH));
                v = float(j / float(HEIGHT));

                r = camera.getRay(u, v);
                color = Color(calculateColorVec(r, world, 0));

            }

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
                        pixels[i][(HEIGHT - 1) - j].r, // This is so we can flip the image horizontally
                        pixels[i][(HEIGHT - 1) - j].g,
                        pixels[i][(HEIGHT - 1) - j].b,
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