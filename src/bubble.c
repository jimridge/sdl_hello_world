#include "bubble.h"

void blit_symmetric_points(SDL_Surface *src_surf, SDL_Surface *target_source, int radius, int x, int y);

SDL_Surface *solid_colour_surface(SDL_Surface *src_surface, SDL_Color colour);

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

SDL_Surface *solid_colour_surface(SDL_Surface *src_surface, SDL_Color colour)
{
    if (!src_surface)
    {
        fprintf(stderr, "Error: Surface is NULL\n");
        return NULL;
    }

    SDL_Surface *colour_surface = SDL_DuplicateSurface(src_surface);

    if (!colour_surface)
    {
        fprintf(stderr, "Error copying surface: %s\n", SDL_GetError());
        return NULL;
    }

    if (!SDL_LockSurface(colour_surface))
    {
        fprintf(stderr, "Error locking surface: %s\n", SDL_GetError());
        SDL_DestroySurface(colour_surface);
        colour_surface = NULL;
        return NULL;
    }

    int width = colour_surface->w;
    int height = colour_surface->h;

    const SDL_PixelFormatDetails *format_details = SDL_GetPixelFormatDetails(colour_surface->format);
    if (!format_details)
    {
        fprintf(stderr, "Error retrieving format details of surface: %s\n", SDL_GetError());
        SDL_DestroySurface(colour_surface);
        colour_surface = NULL;
        return NULL;
    }

    if (format_details->bits_per_pixel != 32)
    {
        fprintf(stderr, "Error with the surface format details. Not 32 bits per pixel\n");
        SDL_DestroySurface(colour_surface);
        colour_surface = NULL;
        return NULL;
    }

    Uint32 *pixels = (Uint32 *)colour_surface->pixels;
    SDL_Palette *palette = SDL_GetSurfacePalette(colour_surface);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Uint32 pixel = pixels[width * y + x];
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixel, format_details, palette, &r, &g, &b, &a);
            if (a > 0)
            {
                pixel = SDL_MapRGBA(format_details, palette, colour.r, colour.g, colour.b, a);
                pixels[width * y + x] = pixel;
            }
        }
    }

    SDL_UnlockSurface(colour_surface);

    return colour_surface;

}

SDL_Surface *bubble_surface(SDL_Surface *src_surface, int radius, SDL_Color outer_colour)
{
    if (!src_surface)
    {
        fprintf(stderr, "Error: Surface is NULL\n");
        return NULL;
    }

    int width = src_surface->w + radius * 2;
    int height = src_surface->h + radius * 2;

    SDL_Surface *target_surface = SDL_CreateSurface(width, height, src_surface->format);

    if (!target_surface)
    {
        fprintf(stderr, "Error creating new surface: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Surface *outer_surface = solid_colour_surface(src_surface, outer_colour);

    if (!outer_surface)
    {
        fprintf(stderr, "Error creating new Surface: %s\n", SDL_GetError());
        SDL_DestroySurface(target_surface);
        target_surface = NULL;
        return NULL;
    }


// Polar Coordinate Trigonometry Algorithm
    // for (int index = 0; index < (2 * M_PI * radius); index++)
    // {
    //     double rad = (double)index / radius;
    //     int x = (int)(cos(rad) * radius) + radius;
    //     int y = (int)(sin(rad) * radius) + radius;
    //     SDL_BlitSurface(outer_surface, NULL, target_surface, &(SDL_Rect){x, y, outer_surface->w, outer_surface->h});
    // }

    // Bresenhams Circle Drawing Algorithm
    int x = 0, y = radius, d = 3 - 2 * radius;
    blit_symmetric_points(outer_surface, target_surface, radius, x, y);
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
        blit_symmetric_points(outer_surface, target_surface, radius, x, y);
    }

    SDL_BlitSurface(src_surface, NULL, target_surface, &(SDL_Rect){radius, radius, src_surface->w, src_surface->h});

    SDL_DestroySurface(outer_surface);
    outer_surface = NULL;
    return target_surface;

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

    SDL_Surface *text_surf = TTF_RenderText_Blended(font, str, 0, inner_colour);
    TTF_CloseFont(font);
    font = NULL;
    if (!text_surf)
    {
        fprintf(stderr, "Error rendering text to surface: %s\n", SDL_GetError());
        TTF_CloseFont(font);
        font = NULL;
        return NULL;
    }


    SDL_Surface *bubble_surf = bubble_surface(text_surf, radius, outer_colour);
    SDL_DestroySurface(text_surf);
    text_surf = NULL;
    if (!bubble_surf)
    {
        return NULL;
    }

    return bubble_surf;
}