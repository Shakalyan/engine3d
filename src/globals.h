#ifndef GLOBALS_GUARD
#define GLOBALS_GUARD

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define TRUE 1
#define FALSE 0

typedef struct sdl {
    SDL_Window *window;
    SDL_Surface *window_surface;
    SDL_Renderer *renderer;
} t_sdl;

typedef struct game {
    char is_exited;
    char is_stopped;
} t_game;

typedef struct globals {
    t_sdl *sdl;
    t_game *game;
} t_globals;

t_globals *init_globals(char *title, int window_xpos, int window_ypos);

void free_globals(t_globals *globals);

#endif