#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#include "SDL2/SDL.h"
#include "../utils/Vector.cpp"
#include "../utils/Color.cpp"

using namespace std;

int main() {

    int X = 200;
    int Y = 200;

    Vector vectors[X][Y];

    ofstream newFrame;
    newFrame.open("./frames/frame0.ppm", ios::out);
    // Visit PPM File format referrence for why is this here
    newFrame << "P3\n" << X << " " << Y << "\n255\n";
    
    // Test for outputing frames
    // TODO implement Material class
    for (int i = Y - 1; i >= 0; i--) {
        for (int j = 0; j < X; j++) {

            float r = float(j) / float(X);
            float g = float(i) / float(Y); 
            float b = 0.5;

            int ir = int(255.99*r);
            int ig = int(255.99*g);
            int ib = int(255.99*b);

            Vector newVector = Vector(float(i), float(j), float(i*j));
            newVector.setColor(Color(float(ir), float(ig), float(ib)));

            vectors[i][j] = newVector;

            newFrame << newVector.color.r << " " << newVector.color.g << " " << newVector.color.b << "\n";   
        }
    }

    newFrame.close();

    // Testing classes and implementation

    for (int i = Y - 1; i >= 0; i--) {
        for (int j = 0; j < X; j++) {
           cout << vectors[i][j].color.r << " " << vectors[i][j].color.g << " " << vectors[i][j].color.b << endl;
        }
    }

    Vector testVec = Vector(5,12,18);
    testVec.transformToUnit();

    cout << testVec.x << " " << testVec.y << " " << testVec.z << endl;

    // SDL Window SETUP
    //The window we'll be rendering to
    SDL_Window* window = NULL;
    
    //The surface contained by the window
    SDL_Surface* screenSurface = NULL;

    //Initialize SDL
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    } else {
        window = SDL_CreateWindow( "PTS3D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, X, Y, SDL_WINDOW_SHOWN );
        if ( window == NULL ) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        } else {
            //Get window surface
            screenSurface = SDL_GetWindowSurface( window );
            //Fill the surface white
            SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFA, 0xFF, 0xFF ) );
            //Update the surface
            SDL_UpdateWindowSurface( window );
            //Wait two seconds
            SDL_Delay( 2000 );
        }
    }

    return 0;
}