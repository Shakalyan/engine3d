#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include "globals.h"
#include "utils.h"
#include "engine3d.h"
#include "render.h"

void handle_input(t_game *game, SDL_Event *event) {
    if (event->type == SDL_QUIT) {
        game->is_exited = TRUE;
        return;
    }

    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_SPACE:
                game->is_stopped = TRUE;
                break;
        }
    } else if (event->type == SDL_KEYUP) {
        switch (event->key.keysym.sym) {
            case SDLK_SPACE:
                game->is_stopped = FALSE;
                break;
        }
    }
}

int main(int argc, char *args[]) {   

    float screen_width = 1920.0;
    float screen_height = 1080.0;

    t_globals *globals = init_globals("My window", screen_width, screen_height);
    if (globals == NULL) {
        printf("Init error: %s", SDL_GetError());
        return 1;
    }
    t_e3d_scene *scene = init_e3d(0.1, 1000.0, 90.0, screen_height / screen_width);

    t_vector3 objpos = {1, 2, 3};
    t_obj3d *cube = load_obj("./media/sphere.obj", 100.0, objpos);

    int buffers_max_size = cube->mesh_size * 3;
    float *render_buffer = malloc(sizeof(float) * buffers_max_size);
    SDL_Color *color_buffer = malloc(sizeof(SDL_Color) * buffers_max_size);
    float *zbuffer = malloc(sizeof(float) * buffers_max_size);
    int *ibuffer = malloc(sizeof(int) * buffers_max_size);

    //t_vector3 movevec = {0, -10, 10};

    float angle = 0.0;
    SDL_Event e;
    while (!globals->game->is_exited) {
        while (SDL_PollEvent(&e) != 0) {
            handle_input(globals->game, &e);
        }
        clear_screen(globals->sdl->renderer);
        SDL_SetRenderDrawColor(globals->sdl->renderer, 0, 0, 0, 0xFF);      
        
        rotate_obj3d(cube, angle*0.5, X_AXIS);
        rotate_obj3d(cube, angle*0.25, Y_AXIS);
        rotate_obj3d(cube, angle, Z_AXIS);
        //moveobjtest(cube, &movevec);
        render_obj3d(cube);
        
        draw_object(globals->sdl->renderer, cube, scene, render_buffer, color_buffer, zbuffer, ibuffer);
        //draw_mesh(globals->sdl->renderer, cube);
        refreshmesh(cube);
        
        if (!globals->game->is_stopped) {
            angle += 0.001;  
            //angle += 0.1;
        }

        SDL_RenderPresent(globals->sdl->renderer);
        //sleep(1);
    }

    DE_Obj3d(cube);
    free_globals(globals);
    return 0;
}