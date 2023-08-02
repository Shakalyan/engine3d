#ifndef LOGIC3D_GUARD
#define LOGIC3D_GUARD

#include <stdlib.h>
#include <stdio.h>

typedef struct vector3 {
    float x;
    float y;
    float z;
} t_vector3;

typedef struct matrix3 {
    t_vector3 vecs[3];
    char display;
    t_vector3 normal;
} t_matrix3;

t_matrix3 *Matrix3(t_vector3 vecs[3]);

void copyvec3(t_vector3 *src, t_vector3 *dst);

void copytomx(t_matrix3 *mx, t_vector3 vecs[3]);

void copymesh(t_matrix3 **srcmesh, t_matrix3 **dstmesh, int mesh_size);

void vecmx3mul(t_vector3 *vec, t_matrix3 *mx, t_vector3 *resv);

void mx3sum(t_matrix3 *mx1, t_matrix3 *mx2, t_matrix3 *res);

void mx3mul(t_matrix3 *mx1, t_matrix3 *mx2, t_matrix3 *res);

void vec3sub(t_vector3 *vec1, t_vector3 *vec2, t_vector3 *res);

float vec3scmul(t_vector3 *vec1, t_vector3 *vec2);

void print_matrix(t_matrix3 *mx);

#endif