#ifndef UTILS_GUARD
#define UTILS_GUARD

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Surface *load_surface(SDL_Surface *window_surface, char *path);

SDL_Texture *load_texture(SDL_Renderer *renderer, char *path);

void move(SDL_Rect *rect, SDL_Point *vector, double scaling);

void clear_screen(SDL_Renderer *renderer);

#endif