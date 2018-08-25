#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#include "SDL2/SDL.h"
#include "Vector.cpp"
#include "Color.cpp"
#include "Ray.cpp"
using namespace std;

static const int X = 1280;
static const int Y = 720;

static Color pixels[X][Y];

Color calculateColor(const Ray& ray) {
    Vector unitDirection = ray.direction();
    unitDirection.transformToUnit(); //map between 0 and 1 so I can use it for colors

    float t = 0.5 * (unitDirection.y + 1.0);
    //using vectors to calculate the linear interpolation between colors, since I already have operator overloads for them
    Vector color = ( 1.0 - t ) * Vector(1.0, 1.0, 1.0) + t * Vector(0.5, 0.7, 1.0);

    int ir = int(255.99*color.x);
    int ig = int(255.99*color.y);
    int ib = int(255.99*color.z);

    return Color(ir, ig, ib);
}

int main() {

    printf( "Initializing...\n");
    // TODO implement Material class

    Vector lowerLeftCorner (-2.0, -1.0, -1.0);
    Vector horizontal      (4.0, 0.0, 0.0);
    Vector vertical        (0.0, 2.0, 0.0);
    Vector origin          (0.0, 0.0, 0.0);

    for (int j = 0; j < Y; j++) {
        for (int i = 0; i < X; i++) {

            float u = float(i) / float(X);
            float v = float(j) / float(Y);

            Ray r(origin, lowerLeftCorner + u*horizontal + v*vertical);
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
        SDL_CreateWindowAndRenderer(X, Y, 0, &window, &renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        if ( window == NULL ) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        } else {
            //Render a all vectors
            for (int j = 0; j < Y; j++) {
                for (int i = 0; i < X; i++) {
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
            //while(true) {
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