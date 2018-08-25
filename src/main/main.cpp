#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#include "SDL2/SDL.h"
#include "../utils/Vector.cpp"
#include "../utils/Color.cpp"

using namespace std;

static const int X = 600;
static const int Y = 300;

static Vector vectors[X][Y];

int main() {

    printf( "Initializing...\n");
    // Test
    // TODO implement Material class
    for (int i = 0; i < X; i++) {
        for (int j = 0; j < Y; j++) {

            float r = float(j) / float(Y);
            float g = float(i) / float(X);
            float b = 0.2;

            int ir = int(255.99*r);
            int ig = int(255.99*g);
            int ib = int(255.99*b);

            Vector newVector = Vector(float(i), float(j), float(i*j));
            newVector.setColor(Color(float(ir), float(ig), float(ib)));

            vectors[i][j] = newVector;
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
            for (int i = 0; i < X; i++) {
                for (int j = 0; j < Y; j++) {
                    SDL_SetRenderDrawColor(
                        renderer,
                        vectors[i][j].color.r,
                        vectors[i][j].color.g,
                        vectors[i][j].color.b,
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