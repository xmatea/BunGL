#ifndef SDL_SETUP_H
#define SDL_SETUP_H
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "SDL_setup.h"

#define SCREEN_WIDTH 383
#define SCREEN_HEIGHT 215

SDL_Window* gWindow;
SDL_Renderer* gRenderer;
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();
#endif