#ifndef MAIN_H
#define MAIN_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define SDL_FLAGS SDL_INIT_VIDEO

#define WINDOW_TITLE "SDL Hello World"
#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540
#define FPS 60

#define TEXT_SIZE 80
#define TEXT_COLOUR (SDL_Color){255,255,255,255}
#define TEXT_STR "SDL"


#endif