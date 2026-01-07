#ifndef BUBBLE_H
#define BUBBLE_H


#include "main.h"

SDL_Surface *bubble_surface(SDL_Surface *src_surface, int radius, SDL_Color outer_colour);
SDL_Surface *bubble_create_text(
    const char *str,
    float size,
    int radius,
    SDL_Color inner_colour,
    SDL_Color outer_colour
);



#endif