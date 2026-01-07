#include "player.h"
#include "bubble.h"
#include "main.h"


bool player_new(struct Player **player, SDL_Renderer *renderer)
{
    *player = calloc(1, sizeof(struct Player));
    if (*player == NULL)
    {
        fprintf(stderr, "Error in CALLOC of New Player\n");
        return false;
    }
    struct Player *p = *player;

    p->renderer = renderer;

    // p->image = IMG_LoadTexture(p->renderer, "assets/images/C-logo.png");

    // if (!p->image)
    // {
    //     fprintf(stderr, "Error loading player texture: %s\n", SDL_GetError());
    //     return false;
    // }

    SDL_Surface *surf = IMG_Load("assets/images/C-logo.png");

    if (!surf)
    {
        fprintf(stderr, "Error loading surface: %s\n", SDL_GetError());
        return false;
    }

    SDL_Surface *bubble_surf = bubble_surface(surf, BUBBLE_RADIUS, WHITE_COLOUR);

    SDL_DestroySurface(surf);
    surf = NULL;
    if (!bubble_surf)
    {
        return false;
    }

    p->image = SDL_CreateTextureFromSurface(p->renderer, bubble_surf);

    SDL_DestroySurface(bubble_surf);
    bubble_surf = NULL;
    if (!p->image)
    {
        fprintf(stderr, "Error creating texture from surface: %s\n", SDL_GetError());
        return false;
    }

    if (!SDL_GetTextureSize(p->image, &p->rect.w, &p->rect.h))
    {
        fprintf(stderr, "Error getting texture size - player: %s\n", SDL_GetError());
        return false;
    }

    p->keystate = SDL_GetKeyboardState(NULL);

    return true;
}


void player_free(struct Player **player)
{
    if (*player)
    {
        struct Player *p = *player;

        if (p->image)
        {
            SDL_DestroyTexture(p->image);
            p->image = NULL;
        }

        p->renderer = NULL;
        p->keystate = NULL;

        free(p);
        p = NULL;
        *player = NULL;

        printf("Free player complete.\n");
    }
}


void player_update(struct Player *p)
{
    if (p->keystate[SDL_SCANCODE_LEFT] || p->keystate[SDL_SCANCODE_A])
    {
        p->rect.x -= PLAYER_VELOCITY;
    }
    if (p->keystate[SDL_SCANCODE_RIGHT] || p->keystate[SDL_SCANCODE_D])
    {
        p->rect.x += PLAYER_VELOCITY;
    }
    if (p->keystate[SDL_SCANCODE_UP] || p->keystate[SDL_SCANCODE_W])
    {
        p->rect.y -= PLAYER_VELOCITY;
    }
    if (p->keystate[SDL_SCANCODE_DOWN] || p->keystate[SDL_SCANCODE_S])
    {
        p->rect.y += PLAYER_VELOCITY;
    }
}


void player_draw(const struct Player *p)
{
    SDL_RenderTexture(p->renderer, p->image, NULL, &p->rect);
}

