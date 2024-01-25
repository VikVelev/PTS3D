#include <fstream>
#include <iostream>
#include <omp.h>
#include <signal.h>
#include <stdio.h>
#include <string>
#include <unistd.h>

#include "Color.cpp"
#include "Ray.cpp"
#include "SDL2/SDL.h"
#include "Vector.cpp"

#include "../utils/Camera.h"
#include "../utils/Geometry.h" //Important functions such as calculateColor
#include "../utils/Material.h"

using namespace std;

static const int WIDTH = 1024;
static const int HEIGHT = 576;
static const int SAMPLES = 1000; // SPP

// SDL Window SETUP
// The window we'll be rendering to
SDL_Window *window = NULL;
SDL_Event event;
SDL_Renderer *renderer;

// The surface contained by the window
SDL_Surface *screenSurface = NULL;

Color pixels[WIDTH][HEIGHT]; // basically the screen
Vector pixelsColorStore[WIDTH][HEIGHT]; // for dynamic spp
                                        // rendering

Hitable *generateRandomScene(int numberOfSpheres) {
  Hitable **objects = new Hitable *[numberOfSpheres + 1];
  objects[0] =
      new Sphere(Vector(0, -1000, 0), 1000,
                 new Lambertian(Vector(0.4, 0.4, 0.4)));
  int i = 1;
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      float chooseMat = drand48();
      Vector center(a + 0.9 * drand48(), 0.2,
                    b + 0.9 * drand48());

      if ((center - Vector(4, 0.2, 0)).length() > 0.9) {
        if (chooseMat < 0.8) {
          objects[i++] =
              new Sphere(center, 0.2,
                         new Lambertian(Vector(
                             drand48() * drand48(),
                             drand48() * drand48(),
                             drand48() * drand48())));
        } else if (chooseMat < 0.95) {
          objects[i++] = new Sphere(
              center, 0.2,
              new Metal(Vector(0.5 * (1 + drand48()),
                               0.5 * (1 + drand48()),
                               0.5 * (1 + drand48())),
                        0.5 * drand48()));
        } else {
          objects[i++] =
              new Sphere(center, 0.2, new Dielectric(1.5));
        }
      }
    }
  }

  objects[i++] =
      new Sphere(Vector(0, 1, 0), 1.0, new Dielectric(1.5));
  objects[i++] =
      new Sphere(Vector(-4, 1, 0), 1.0,
                 new Lambertian(Vector(0.5, 0.2, 0.1)));
  objects[i++] =
      new Sphere(Vector(4, 1, 0), 1.0,
                 new Metal(Vector(0.7, 0.6, 0.5), 0.0));

  return new Scene(objects, i);
}

inline void renderScene() {
  for (int j = 0; j < HEIGHT; j++) {
    for (int i = 0; i < WIDTH; i++) {
      SDL_SetRenderDrawColor(
          renderer,
          // This is so we can flip the image horizontally
          (Uint8)pixels[i][(HEIGHT - 1) - j].r,
          (Uint8)pixels[i][(HEIGHT - 1) - j].g,
          (Uint8)pixels[i][(HEIGHT - 1) - j].b, 255);
      // printf("%d %d \n", i, j);
      SDL_RenderDrawPoint(renderer, i, j);
    }
  }
  SDL_RenderPresent(renderer);
}

int main() {

  printf("Initializing...\n");

  float ratio = float(WIDTH) / float(HEIGHT);
  // Build Scene

  Hitable *hitable[5];

  hitable[0] = new Sphere(
      Vector(0, 0, -1),                     // center
      0.5f,                                 // radius
      new Lambertian(Vector(0.5, 0.7, 1.0)) // color (blue)
  );

  hitable[1] =
      new Sphere(Vector(0, -100.5f, -1), // center
                 100.0f,                 // radius
                 new Lambertian(Vector(1.f, 0.f, 0.0)));

  // Glass ball
  hitable[2] = new Sphere(Vector(-1, 0, -1.1), // center
                          -0.45f,              // radius
                          new Dielectric(1.5));

  hitable[3] = new Sphere(Vector(-1, 0, -1.1), // center
                          0.5f,                // radius
                          new Dielectric(1.5));
  // end glass ball

  hitable[4] =
      new Sphere(Vector(1, 0, -1), // center
                 0.5f,             // radius
                 new Metal(Vector(0.8, 0.6, 0.2), 0.2));
  // float R = cos(PI/4);
  // hitable[0] = new Sphere(Vector(-R,0,-1), R, new
  // Lambertian(Vector(0,0,1))); hitable[1] = new
  // Sphere(Vector(R,0,-1), R, new
  // Lambertian(Vector(1,0,0)));

  Hitable *world = generateRandomScene(500);

  Vector lookFrom(8, 2, 2);
  Vector lookAt(3.5, 1, 0);

  Camera camera(lookFrom, lookAt, Vector(0, 1, 0), 30,
                ratio, 0.2, (lookFrom - lookAt).length());

  printf("Calculating...\n");

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n",
           SDL_GetError());
  } else {
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window,
                                &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
  }

  time_t startTime;
  time(&startTime);

  for (int s = 0; s < SAMPLES; s++) {
#pragma omp parallel for
    for (int j = 0; j < HEIGHT; j++) { // from right to left
      for (int i = 0; i < WIDTH; i++) { // from up to down
#pragma omp task shared(camera, world)
        {
          if (SDL_PollEvent(&event) &&
              event.type == SDL_QUIT) {
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

          pixelsColorStore[i][j] +=
              calculateColorVec(r, world, 0);
          color = Color((pixelsColorStore[i][j] / float(s + 1)));

          pixels[i][j] = color;
        }
      }
    }

    renderScene();
    printf("Current SPP %d\n", s);
  }

  printf("Done in %d seconds\n", time(0) - startTime);

  for (;;) {
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
