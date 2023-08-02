#include "render.h"

void draw_poly(SDL_Renderer *renderer, t_matrix3 *tri) {
    for (int i = 0; i < 3; ++i) {
        for (int j = i+1; j < 3; ++j) {
            SDL_RenderDrawLineF(renderer,
                                tri->vecs[i].x,
                                tri->vecs[i].y,
                                tri->vecs[j].x,
                                tri->vecs[j].y);
        }
    }
}

void draw_mesh(SDL_Renderer *renderer, t_obj3d *obj) {
    for (int i = 0; i < obj->mesh_size; ++i) {
        if (obj->rendermesh[i]->display)
            draw_poly(renderer, obj->rendermesh[i]);
    }
}

void set_color(SDL_Color *color, int r, int g, int b, int a) {
    color->r = r;
    color->g = g;
    color->b = b;
    color->a = a;
}

void swapf(float *f, float *s) {
    float tmp = *f;
    *f = *s;
    *s = tmp;
}

void swapi(int *f, int *s) {
    int tmp = *f;
    *f = *s;
    *s = tmp;
}

void z_depth_sort(float *zbuffer, int *ibuffer, int l, int r) {
    if (l >= r)
        return;
    float pivotz = zbuffer[(r + l) / 2];
    int midi = 0;
    for (int i = l, j = r;;) {
        while (zbuffer[i] > pivotz) ++i;
        while (zbuffer[j] < pivotz) --j;
        if (i >= j) {
            midi = j;
            break;
        }
        swapf(&zbuffer[i], &zbuffer[j]);
        swapi(&ibuffer[i*3], &ibuffer[j*3]);
        swapi(&ibuffer[i*3+1], &ibuffer[j*3+1]);
        swapi(&ibuffer[i*3+2], &ibuffer[j*3+2]);
        ++i; --j;
    }
    z_depth_sort(zbuffer, ibuffer, l, midi);
    z_depth_sort(zbuffer, ibuffer, midi+1, r);
}

void z_depth(float *zbuffer, int *ibuffer, int bufsize) {
    for (int i = 0; i < bufsize * 3; ++i) {
        ibuffer[i] = i;
    }
    z_depth_sort(zbuffer, ibuffer, 0, bufsize-1);
}

void draw_object(SDL_Renderer *renderer, t_obj3d *obj, t_e3d_scene *scene, float *vert_buffer, SDL_Color *color_buffer,
                  float *zbuffer, int *ibuffer) {
    int vidx = 0, cidx = 0, displayed = 0;
    float col = 0;
    for (int mx = 0; mx < obj->mesh_size; ++mx) {
        if (!obj->rendermesh[mx]->display)
            continue;
        ++displayed;
        for (int v = 0; v < 3; ++v) {
            vert_buffer[vidx] = obj->rendermesh[mx]->vecs[v].x;
            vert_buffer[vidx+1] = obj->rendermesh[mx]->vecs[v].y;
            col = vec3scmul(&scene->lighting.direction, &obj->rendermesh[mx]->normal);
            char color = 128 - 128 * col;
            set_color(&color_buffer[cidx], color, color, color, 255);
            vidx += 2;
            ++cidx;
        }
        float avz = (obj->rendermesh[mx]->vecs[0].z + obj->rendermesh[mx]->vecs[1].z + obj->rendermesh[mx]->vecs[2].z) / 3.0;
        zbuffer[displayed-1] = avz;
    }
    z_depth(zbuffer, ibuffer, displayed);
    SDL_RenderGeometryRaw(renderer, NULL, vert_buffer, sizeof(float)*2, color_buffer, sizeof(SDL_Color), NULL, 0, displayed*3, ibuffer, displayed * 3, sizeof(int));
}