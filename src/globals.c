#include "globals.h"
#include <stdlib.h>

t_globals *init_globals(char *title, int screen_width, int screen_height) {    
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || IMG_Init(IMG_INIT_PNG) < 0) {
        return NULL;
    }

    SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        return NULL;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        return NULL;
    }
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    t_sdl *sdl = malloc(sizeof(t_sdl));
    sdl->window = window;
    sdl->window_surface = SDL_GetWindowSurface(sdl->window);
    sdl->renderer = renderer;

    t_game *game = malloc(sizeof(t_game));
    game->is_exited = FALSE;
    game->is_stopped = FALSE;

    t_globals *globals = malloc(sizeof(t_globals));
    globals->sdl = sdl;
    globals->game = game;

    return globals;
}

void free_globals(t_globals *globals) {
    SDL_DestroyWindow(globals->sdl->window);
    SDL_DestroyRenderer(globals->sdl->renderer);
    SDL_Quit();
    free(globals->sdl);
    free(globals->game);
    free(globals);
}