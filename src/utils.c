#include "utils.h"

SDL_Surface *load_surface(SDL_Surface *window_surface, char *path) {
    SDL_Surface *surface = IMG_Load(path);
    if (surface == NULL) {
        return NULL;
    }

    SDL_Surface *optimized = SDL_ConvertSurface(surface, window_surface->format, 0);
    SDL_FreeSurface(surface);
    return optimized;
}

SDL_Texture *load_texture(SDL_Renderer *renderer, char *path) {
    SDL_Surface *surface = IMG_Load(path);
    if (surface == NULL) {
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void move(SDL_Rect *rect, SDL_Point *vector, double scaling) {
    rect->x += vector->x * scaling;
    rect->y += vector->y * scaling;
}

void clear_screen(SDL_Renderer *renderer) {
    static char color = 0x00;
    SDL_SetRenderDrawColor(renderer, color, color, color, 0xFF);
    SDL_RenderClear(renderer);
}