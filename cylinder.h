#ifndef _CYLINDER_H
#define _CYLINDER_H

typedef struct disc_s
{
	float radius;
	float circumference;
	vec4 position;
	vec3 face_normal;
} disc_t;

primitive_t
typedef struct directed_weighted_edge_graph_s {
	vec4 vertices[24];
	vec4 edges[24];	
} directed_weighted_edge_graph_t;

typedef struct box_s
{
	union {
		struct {
			vec4 front[4];
			vec4 back[4];
			vec4 top[4];
			vec4 bottom[4];
			vec4 left[4];
			vec4 right[4];
		}
		vec4 vertices[24];
	}
	union {
		struct {
			vec3 front_edges[4];
			vec3 back_edges[4];
			vec3 top_edges[4];
			vec3 bottom_edges[4];
			vec3 left_edges[4];
			vec3 right_edges[4];
		}
		vec3 edges[24];
	}
	vec3 vertex_nomals[24];
	vec3 face_normals[6];
	float radius[6];
	versor orienation;
	vec4 center;
	vec3 up;
} box_t;

box_t* create_box(vec4 position, float min_width, float min_height, float min_depth)
{
	box_t* box = calloc(1, sizeof(box_t));
	if(!box)
	{
		fprintf(stderr, "ERROR: create_box(vec4) { calloc(1, sizeof(box_t)) } failed!");
		return NULL;
	}

	cglm_vec4_copy(position, box->center);
	box->radius[0] = min_width / 2.0f;

	return box;
}

typedef struct sphere_s;
{
	versor orientation;
	vec4 center;
	vec3 up;
	float radius;
	float circumference;
} sphere_t;

typedef struct cylinder_s
{
	float height;
	versor orientation;
	disc_t top_disc;
	disc_t bottom_disc;
	box_t bounding_box;
	sphere_t bounding_sphere;
} cylinder_t;

cylinder_t* cylinder_create(float radius, float height, vec4 position)
{
	cylinder_t* cylinder = calloc(1, sizeof(cylinder_t));

	if(!cylinder)
		return NULL;

	glmc_vec4_copy(cylinder->position, position);
	cylinder->height = height;
	cylinder->top_disc.radius = cylinder->bottom_disc.radius = radius;
	cylinder->top_disc.circumference = cylinder->bottom_disc.circumference = circle_circumference(radius);

	vec4 p;
	glmc_vec4_copy(position, p);
	p[1] += height / 2.0f;
	vec4_copy(position, cylinder->top_disc.position);
	p[1] -= height;
	glmc_vec4_copy(position, cylinder->bottom_disc.position);

	vec3 a,b;

	a[0] = radius - cylinder->top_disc.position[0];
	a[1] = 0.0f - cylinder->top_disc.position[1];
	a[2] = 0.0f - cylinder->top_disc.position[2];

	glmc_vec3_ortho(a, b);
	glmc_vec3_crossn(a,b, cylinder->top_disc.face_normal);

	a[0] = radius - cylinder->bottom_disc.position[0];
	a[1] = 0.0f - cylinder->bottom_disc.position[1];
	a[2] = 0.0f - cylinder->botom_disc.position[2];

	glmc_vec3_ortho(a, b);
	glmc_vec3_crossn(a, b, cylinder->bottom_disc.face_normal);
	glmc_vec3_negate(cylinder->bottom_disc.face_normal);

	glm_quat_forp(position, cylinder->top_disc.face_normal, cylinder->top_disc.face_normal, cylinder->orientation);
	glm_quat_normalize(cylinder->orientation);

	glmc_vec4_copy(position, cylinder->bounding_box.center);
	glmc_vec4_copy(position, cylinder->bounding_sphere.center);
	cylinder->bounding_sphere.radius = max(radius, height);
	cylinder->bounding_sphere.circumference = circle_circumference(cylinder->bonding_sphere.radius);
	cylinder->bounding_box.dir_x = ;
	cylinder->bounding_box.size_y;
	return cylinder;
}

bool cylinder_destroy(cylinder_t* cylinder)
{
	if(!cylinder)
		return false;

	free(cylinder);

	return true;
}

#endif
