#include "logic3d.h"

t_matrix3 *Matrix3(t_vector3 vecs[3]) {
    t_matrix3 *mx = malloc(sizeof(t_matrix3));
    copytomx(mx, vecs);
    return mx;
}

void copyvec3(t_vector3 *src, t_vector3 *dst) {
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
}

void copytomx(t_matrix3 *mx, t_vector3 vecs[3]) {
    for (int i = 0; i < 3; ++i) {
        mx->vecs[i] = vecs[i];
    }
}

void copymesh(t_matrix3 **srcmesh, t_matrix3 **dstmesh, int mesh_size) {
    for (int i = 0; i < mesh_size; ++i) {
        copytomx(dstmesh[i], srcmesh[i]->vecs);
    }
}

void vecmx3mul(t_vector3 *vec, t_matrix3 *mx, t_vector3 *resv) {
    float x, y, z;
    x = vec->x * mx->vecs[0].x + vec->y * mx->vecs[1].x + vec->z * mx->vecs[2].x;
    y = vec->x * mx->vecs[0].y + vec->y * mx->vecs[1].y + vec->z * mx->vecs[2].y;
    z = vec->x * mx->vecs[0].z + vec->y * mx->vecs[1].z + vec->z * mx->vecs[2].z;
    resv->x = x;
    resv->y = y;
    resv->z = z;
}

void vec3sub(t_vector3 *vec1, t_vector3 *vec2, t_vector3 *res) {
    res->x = vec2->x - vec1->x;
    res->y = vec2->y - vec1->y;
    res->z = vec2->z - vec1->z;
}

float vec3scmul(t_vector3 *vec1, t_vector3 *vec2) {
    return vec1->x * vec2->x + vec1->y * vec2->y + vec1->z * vec2->z;
}

void mx3sum(t_matrix3 *mx1, t_matrix3 *mx2, t_matrix3 *res) {
    for (int i = 0; i < 3; ++i) {
        res->vecs[i].x = mx1->vecs[i].x + mx2->vecs[i].x;
        res->vecs[i].y = mx1->vecs[i].y + mx2->vecs[i].y;
        res->vecs[i].z = mx1->vecs[i].z + mx2->vecs[i].z;
    }
}

void mx3mul(t_matrix3 *mx1, t_matrix3 *mx2, t_matrix3 *res) {
    for (int i = 0; i < 3; ++i) {
        vecmx3mul(&mx1->vecs[i], mx2, &res->vecs[i]);
    }
}

void print_matrix(t_matrix3 *mx) {
    for (int i = 0; i < 3; ++i) {
        printf("%f\t%f\t%f\n", mx->vecs[i].x, mx->vecs[i].y, mx->vecs[i].z);
    }
}