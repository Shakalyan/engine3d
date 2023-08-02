#ifndef RENDER_GUARD
#define RENDER_GUARD

#include <SDL2/SDL.h>
#include "engine3d.h"

void draw_poly(SDL_Renderer *renderer, t_matrix3 *tri);

void draw_mesh(SDL_Renderer *renderer, t_obj3d *obj);

void set_color(SDL_Color *color, int r, int g, int b, int a);

void swapf(float *f, float *s);

void swapi(int *f, int *s);

void z_depth_sort(float *zbuffer, int *ibuffer, int l, int r);

void z_depth(float *zbuffer, int *ibuffer, int bufsize);

void draw_object(SDL_Renderer *renderer, t_obj3d *obj, t_e3d_scene *scene, float *vert_buffer, SDL_Color *color_buffer,
                 float *zbuffer, int *ibuffer);

#endif