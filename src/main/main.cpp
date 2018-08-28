#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#include "SDL2/SDL.h"
#include "Vector.cpp"
#include "Color.cpp"
#include "Ray.cpp"

#include "../utils/Geometry.h"
#include "../utils/Camera.h"

using namespace std;

static const int WIDTH = 1200;
static const int HEIGHT = 600;
static const int SAMPLES = 100;

static Color pixels[WIDTH][HEIGHT];

Color calculateColor(const Ray& ray, Hitable *world) {

    hitRecord record;// t parameter of the hit Sphere
    Vector color; 

    if(world->hit(ray, 0.0, MAXFLOAT, record)) {

        color = 0.5f * Vector(
            record.normal.x + 1, 
            record.normal.y + 1, 
            record.normal.z + 1
        ); //the same hack used below to transform t between 0 and 1

    } else {

        Vector unitDirection = ray.direction();
        unitDirection.transformToUnit(); //map between -1 and 1

        float t = 0.5 * (unitDirection.y + 1.0); //hack to get T between 0 and 1 so I can use it for colors

        //using vectors to calculate the linear interpolation between colors, since I already have operator overloads for them
        color = (1.0f - t) * Vector(1.0, 1.0, 1.0) +  t * Vector(0.5, 0.7, 1.0);

    }

    return Color(color);
}

int main() {

    printf( "Initializing...\n");
    // TODO implement Material class

    float ratio = float(WIDTH) / float(HEIGHT);

    Vector lowerLeftCorner (-ratio, -ratio/2, -ratio/2);
    Vector horizontal      (ratio*2,  0.0,  0.0);
    Vector vertical        (0.0,  ratio,  0.0);
    Vector origin          (0.0,  0.0,  0.0);

    //Build Scene

    Hitable *hitable[2];

    hitable[0] = new Sphere(  
        Vector(0.0f, 0.0f, -1.0f), //center
        0.5f, //radius
        Color(255, 0, 0) //color (red)
    );

    hitable[1] = new Sphere(  
        Vector(0.0f, -100.5f, -1.0f), //center
        100.0f, //radius
        Color(0, 255, 0) //color (green)
    );

    Hitable *world = new Scene(hitable, 2);
    Camera camera(ratio);

    for (int j = 0; j < HEIGHT; j++) { // from right to left
        for (int i = 0; i < WIDTH; i++) {   // from up to down

            Vector colorStore(0, 0, 0); //storing color for antialiasing;
            
            //ANTIALIASING
            for (int s = 0; s < SAMPLES; s++) {
                float u = float(i + drand48()) / float(WIDTH);
                float v = float(j + drand48()) / float(HEIGHT);
                
                Ray r = camera.getRay(u, v);
                colorStore += calculateColor(r, world).colorV;
            }

            colorStore /= float(SAMPLES);
            Color color = Color(colorStore);

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
                        pixels[i][(HEIGHT - 1) - j].r, // This is so we can flip the image vertically
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