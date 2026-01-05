#include "init_sdl.h"


bool game_init_sdl(struct Game *g)
{
    if (!SDL_Init(SDL_FLAGS))
    {
        fprintf(stderr, "Error initialising SDL3: %s\n", SDL_GetError());
        return false;
    }

    if (!TTF_Init())
    {
        fprintf(stderr, "Error initialising SDL3_TTF: %s\n", SDL_GetError());
        return false;
    }

    g->window = SDL_CreateWindow(WINDOW_TITLE,WINDOW_WIDTH,WINDOW_HEIGHT,0);

    if (!g->window)
    {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        return false;
    }

    g->renderer = SDL_CreateRenderer(g->window, NULL);

    if (!g->renderer)
    {
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        return false;
    }


    SDL_Surface *icon_surf = IMG_Load("assets/images/C-logo.png");

    if (!icon_surf)
    {
        fprintf(stderr, "Error loading surface: %s\n", SDL_GetError());
        return false;
    }

    if(!SDL_SetWindowIcon(g->window, icon_surf))
    {
        fprintf(stderr, "Error setting window icon: %s\n", SDL_GetError());
        SDL_DestroySurface(icon_surf);
        return false;
    }

    SDL_DestroySurface(icon_surf);
    return true;
}