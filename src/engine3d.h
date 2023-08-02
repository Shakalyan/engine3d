#ifndef ENGINE3D_GUARD
#define ENGINE3D_GUARD

#define PI 3.14159
#include "logic3d.h"

typedef struct e3d_params {
    float front;
    float back;
    float fov;
    float aspect_ratio;
    float rfov;
    float zmul;
    float zsum;
} t_e3d_params;

typedef struct obj3d {
    int mesh_size;
    t_matrix3 **mesh;
    t_matrix3 **rendermesh;
    float scaling;
    t_vector3 position;
} t_obj3d;

typedef struct camera {
    t_vector3 position;
} t_camera;

typedef struct lighting {
    t_vector3 direction;
} t_lighting;

typedef struct e3d_scene {
    t_camera camera;
    t_lighting lighting;
} t_e3d_scene;

typedef enum axis {
    X_AXIS, Y_AXIS, Z_AXIS
} axis;

t_obj3d *Obj3d(t_matrix3 **mesh, int mesh_size, float scaling, t_vector3 position);

void DE_Obj3d(t_obj3d *obj);

t_e3d_scene *init_e3d(float front, float back, float fov, float aspect_ratio);

void rotate_obj3d(t_obj3d *obj, float angle, axis axs);

void render_obj3d(t_obj3d *obj);

void refreshmesh(t_obj3d *obj);

t_obj3d *load_obj(char *path, float scaling, t_vector3 position);

void moveobjtest(t_obj3d *obj, t_vector3 *vec);

#endif