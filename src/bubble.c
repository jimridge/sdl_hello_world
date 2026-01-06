#include "bubble.h"

void blit_symmetric_points(SDL_Surface *src_surf, SDL_Surface *target_surf, int radius, int x, int y)
{

    SDL_BlitSurface(src_surf, NULL, target_surf, &(SDL_Rect){radius + x, radius + y, src_surf->w, src_surf->h});
    SDL_BlitSurface(src_surf, NULL, target_surf, &(SDL_Rect){radius + x, radius - y, src_surf->w, src_surf->h});
    SDL_BlitSurface(src_surf, NULL, target_surf, &(SDL_Rect){radius - x, radius + y, src_surf->w, src_surf->h});
    SDL_BlitSurface(src_surf, NULL, target_surf, &(SDL_Rect){radius - x, radius - y, src_surf->w, src_surf->h});
    SDL_BlitSurface(src_surf, NULL, target_surf, &(SDL_Rect){radius + y, radius + x, src_surf->w, src_surf->h});
    SDL_BlitSurface(src_surf, NULL, target_surf, &(SDL_Rect){radius + y, radius - x, src_surf->w, src_surf->h});
    SDL_BlitSurface(src_surf, NULL, target_surf, &(SDL_Rect){radius - y, radius + x, src_surf->w, src_surf->h});
    SDL_BlitSurface(src_surf, NULL, target_surf, &(SDL_Rect){radius - y, radius - x, src_surf->w, src_surf->h});
}


SDL_Surface *bubble_create_text
(
    const char *str,
    float size,
    int radius,
    SDL_Color inner_colour,
    SDL_Color outer_colour
)
{
    TTF_Font *font = TTF_OpenFont("assets/fonts/freesansbold.ttf", size);

    if (!font)
    {
        fprintf(stderr, "Error opening font: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Surface *surf = TTF_RenderText_Blended(font, str, 0, outer_colour);

    if (!surf)
    {
        fprintf(stderr, "Error rendering text to surface: %s\n", SDL_GetError());
        TTF_CloseFont(font);
        font = NULL;
        return NULL;
    }


    SDL_Surface *target_surface = SDL_CreateSurface(surf->w + radius * 2, surf->h + radius * 2, SDL_PIXELFORMAT_ARGB32);

    if (!target_surface)
    {
        fprintf(stderr, "Error creating new surface: %s\n", SDL_GetError());
        SDL_DestroySurface(surf);
        TTF_CloseFont(font);
        font = NULL;
        return NULL;
    }

    // Polar Coordinate Trigonometry Algorithm
    // for (int index = 0; index < (2 * M_PI * radius); index++)
    // {
    //     double rad = (double)index / radius;
    //     int x = (int)(cos(rad) * radius) + radius;
    //     int y = (int)(sin(rad) * radius) + radius;
    //     SDL_BlitSurface(surf, NULL, target_surface, &(SDL_Rect){x, y, surf->w, surf->h});
    // }

    // Bresenhams Circle Drawing Algorithm
    int x = 0, y = radius, d = 3 - 2 * radius;
    blit_symmetric_points(surf, target_surface, radius, x, y);
    while (y >= x)
    {
        x++;
        if (d > 0)
        {
            y--;
            d = d + 4 * (x-y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }
        blit_symmetric_points(surf, target_surface, radius, x, y);
    }

    SDL_DestroySurface(surf);
    surf = NULL;
    surf = TTF_RenderText_Blended(font, str, 0, inner_colour);
    TTF_CloseFont(font);
    font = NULL;
    if (!surf)
    {
        fprintf(stderr, "Error rendering text to surface: %s\n", SDL_GetError());
        SDL_DestroySurface(target_surface);
        return NULL;
    }

    SDL_BlitSurface(surf, NULL, target_surface, &(SDL_Rect){radius, radius, surf->w, surf->h});

    SDL_DestroySurface(surf);
    surf = NULL;
    return target_surface;
}