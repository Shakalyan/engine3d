#include "engine3d.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef struct e3d_globals {
    t_e3d_params e3d_params;
    t_matrix3 scalemx;
    t_matrix3 helpmx;
    t_e3d_scene scene;
} t_e3d_globals;
t_e3d_globals *e3dg;

t_e3d_scene *init_e3d(float front, float back, float fov, float aspect_ratio) {
    e3dg = malloc(sizeof(t_e3d_globals));
    e3dg->e3d_params.front = front;
    e3dg->e3d_params.back = back;
    e3dg->e3d_params.fov = fov;
    e3dg->e3d_params.aspect_ratio = aspect_ratio;
    e3dg->e3d_params.rfov = 1.0 / tanf(fov * 0.5 * PI / 180.0);
    e3dg->e3d_params.zmul = back / (back - front);
    e3dg->e3d_params.zsum = (-back * front) / (back - front);

    t_vector3 camerapos = {0, 0, 0};
    copyvec3(&camerapos, &e3dg->scene.camera.position);

    t_vector3 lighting_dir = {1, 0, 0};
    copyvec3(&lighting_dir, &e3dg->scene.lighting.direction);

    return &e3dg->scene;
}

t_obj3d *Obj3d(t_matrix3 **mesh, int mesh_size, float scaling, t_vector3 position) {
    t_obj3d *obj = malloc(sizeof(t_obj3d));
    obj->scaling = scaling;
    obj->position = position;
    obj->mesh_size = mesh_size;

    obj->mesh = malloc(sizeof(t_matrix3*) * mesh_size);
    obj->rendermesh = malloc(sizeof(t_matrix3*) * mesh_size);

    for (int i = 0; i < mesh_size; ++i) {
        obj->mesh[i] = malloc(sizeof(t_matrix3));
        obj->rendermesh[i] = malloc(sizeof(t_matrix3));
    }

    copymesh(mesh, obj->mesh, mesh_size);
    copymesh(mesh, obj->rendermesh, mesh_size);

    return obj;
}

static void get_obj_sizes(FILE *f, int *vrt_count, int *tri_count) {
    char buffer[255];
    *vrt_count = 0;
    *tri_count = 0;
    while (fgets(buffer, 255, f)) {
        if (buffer[0] == 'v')
            *vrt_count += 1;
        else if (buffer[0] == 'f')
            *tri_count += 1;
    }
}

t_obj3d *load_obj(char *path, float scaling, t_vector3 position) {
    FILE* f = fopen(path, "r");
    
    int vrt_count, tri_count;
    get_obj_sizes(f, &vrt_count, &tri_count);
    rewind(f);
    
    t_obj3d *obj = malloc(sizeof(t_obj3d));
    obj->mesh = malloc(sizeof(t_matrix3*) * tri_count);
    obj->rendermesh = malloc(sizeof(t_matrix3*) * tri_count);
    obj->mesh_size = tri_count;
    obj->scaling = scaling;
    obj->position = position;

    char buffer[255];
    int skip = 3;
    for (int i = 0; i < skip; ++i)
        fgets(buffer, 255, f);
    
    t_vector3 *verts = malloc(sizeof(t_vector3) * vrt_count);
    int cnt = 0;
    char info;
    while (fgets(buffer, 255, f)) {
        if (buffer[0] != 'v')
            break;
        sscanf(buffer, "%c %f %f %f", &info, &verts[cnt].x, &verts[cnt].y, &verts[cnt].z);
        ++cnt;
    }

    int v1, v2, v3;
    cnt = 0;
    while (fgets(buffer, 255, f)) {
        if (buffer[0] != 'f')
            break;
        sscanf(buffer, "%c %d %d %d", &info, &v1, &v2, &v3);
        t_vector3 mx[3] = {verts[v1-1], verts[v2-1], verts[v3-1]};
        obj->mesh[cnt] = Matrix3(mx);
        ++cnt;
    }

    for (int i = 0; i < obj->mesh_size; ++i) {
        obj->rendermesh[i] = malloc(sizeof(t_matrix3));
    }
    copymesh(obj->mesh, obj->rendermesh, obj->mesh_size);

    free(verts);
    fclose(f);
    return obj;
}

void DE_Obj3d(t_obj3d *obj) {
    for (int i = 0; i < obj->mesh_size; ++i) {
        free(obj->mesh[i]);
        free(obj->rendermesh[i]);
    }
    free(obj->mesh);
    free(obj->rendermesh);
    free(obj);
}

void refreshmesh(t_obj3d *obj) {
    copymesh(obj->mesh, obj->rendermesh, obj->mesh_size);
}

static char set_display(t_matrix3 *mx) {
    t_vector3 line1, line2;
    vec3sub(&mx->vecs[0], &mx->vecs[1], &line1);
    vec3sub(&mx->vecs[0], &mx->vecs[2], &line2);
    t_vector3 norm = {
        line1.y * line2.z - line1.z * line2.y,
        line1.z * line2.x - line1.x * line2.z,
        line1.x * line2.y - line1.y * line2.x
    };
    float l = sqrtf(norm.x * norm.x + norm.y * norm.y + norm.z * norm.z);
    norm.x /= l;
    norm.y /= l;
    norm.z /= l;
    float scmul = vec3scmul(&mx->vecs[0], &norm);
    mx->display= scmul < 0;
    mx->normal = norm;
    return mx->display;    
}

static void project3D(t_obj3d *obj) {
    for (int mx = 0; mx < obj->mesh_size; ++mx) {
        if (!set_display(obj->rendermesh[mx])) {
            continue;
        }
        for (int v = 0; v < 3; ++v) {
            t_vector3 *vec = &obj->rendermesh[mx]->vecs[v];
            float w = vec->z;
            vec->x = e3dg->e3d_params.aspect_ratio * e3dg->e3d_params.rfov * vec->x;
            vec->y = e3dg->e3d_params.rfov * vec->y;
            vec->z = e3dg->e3d_params.zmul * vec->z + e3dg->e3d_params.zsum;
            if (w != 0.0) {
                vec->x /= w;
                vec->y /= w;
                vec->z /= w;
            }
        }
    }
}

void rotate_obj3d(t_obj3d *obj, float angle, axis axs) {
    t_vector3 rvecs[3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    float sa = sinf(angle);
    float ca = cosf(angle);
    switch (axs) {
        case X_AXIS:
            rvecs[1].y = ca;
            rvecs[2].z = ca;
            rvecs[1].z = -sa;
            rvecs[2].y = sa;
            rvecs[0].x = 1;
            break;
        case Y_AXIS:
            rvecs[0].x = ca;
            rvecs[2].z = ca;
            rvecs[0].z = -sa;
            rvecs[2].x = sa;
            rvecs[1].y = 1;
            break;
        case Z_AXIS:
            rvecs[0].x = ca;
            rvecs[1].y = ca;
            rvecs[0].y = -sa;
            rvecs[1].x = sa;
            rvecs[2].z = 1;
            break;
    }
    t_matrix3 mx;
    copytomx(&mx, rvecs);
    for (int i = 0; i < obj->mesh_size; ++i) {
        mx3mul(obj->rendermesh[i], &mx, obj->rendermesh[i]);
    }
}

static void scale_object(t_obj3d *obj) {
    t_vector3 scalevecs[3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    scalevecs[0].x = 960;
    scalevecs[1].y = 540;
    scalevecs[2].z = 1;
    copytomx(&e3dg->scalemx, scalevecs);
    for (int i = 0; i < obj->mesh_size; ++i) {
        if (obj->rendermesh[i]->display)
            mx3mul(obj->rendermesh[i], &e3dg->scalemx, obj->rendermesh[i]);
    }
}

static void move_objectz(t_obj3d *obj, float zofs) {
    for (int i = 0; i < 3; ++i) {
        e3dg->helpmx.vecs[i].x = 0;
        e3dg->helpmx.vecs[i].y = 0;
        e3dg->helpmx.vecs[i].z = zofs;
    }
    for (int i = 0; i < obj->mesh_size; ++i) {
        mx3sum(obj->rendermesh[i], &e3dg->helpmx, obj->rendermesh[i]);
    }
}

static void offset_obj(t_obj3d *obj, float offset) {
    for (int i = 0; i < 3; ++i) {
        e3dg->helpmx.vecs[i].x = offset;
        e3dg->helpmx.vecs[i].y = offset;
        e3dg->helpmx.vecs[i].z = 1;
    }
    for (int i = 0; i < obj->mesh_size; ++i) {
        if (obj->rendermesh[i]->display)
            mx3sum(obj->rendermesh[i], &e3dg->helpmx, obj->rendermesh[i]);
    }
}

void moveobjtest(t_obj3d *obj, t_vector3 *vec) {
    for (int i = 0; i < obj->mesh_size; ++i) {
        for (int j = 0; j < 3; ++j) {
            obj->rendermesh[i]->vecs[j].x += vec->x;
            obj->rendermesh[i]->vecs[j].y += vec->y;
            obj->rendermesh[i]->vecs[j].z += vec->z;
        }
    }
}

void render_obj3d(t_obj3d *obj) {
    move_objectz(obj, 3.0);
    project3D(obj);
    offset_obj(obj, 1.0);
    scale_object(obj);    
}