#ifndef BUBBLE_H
#define BUBBLE_H


#include "main.h"


SDL_Surface *bubble_create_text(
    const char *str,
    float size,
    int radius,
    SDL_Color inner_colour,
    SDL_Color outer_colour
);

void blit_symmetric_points(SDL_Surface *src_surf, SDL_Surface *target_source, int radius, int x, int y);

#endif