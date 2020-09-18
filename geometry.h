#ifndef _GEOMETRY_H
#define _GEOMETRY_H

#include "system_types.h"

typedef struct disc_s
{
	float radius;
	float circumference;
	vec4 position;
	vec3 face_normal;
} disc_t;


typedef struct directed_weighted_edge_graph_s {
	vec4 vertices[24];
	vec4 edges[24];	
} directed_weighted_edge_graph_t;

typedef struct triangle_s
{
	vec4 vertices[3];
	vec4 edges[3];
	vec3 vertex_normals[3];
	vec3 face_normal;
} triangle_t;

typedef struct triangle_indexed_s
{
	size_t vertex_indices[3];
	size_t edge_indices[3];
	vec4* vertices;
	vec3* vertex_normals;
	vec4* edges;
	vec3 face_normal;
} triangle_indexed_t;

typedef struct quad_s
{
	vec4 vertices[4];
	vec4 edges[6];
	vec3 vertex_normals[4];
	vec3 face_normal;
	triangle_indexed_t triangles[2];
} quaad_t;

triangle_indexed_t* triangle_indexed_create(quad_t* quad)
{
	triangle_indexed_t* triangle = calloc(2, sizeof(triangle_indexed_t));

	triangle[0].vertex_indices[0] = 0;
	triangle[0].vertex_indices[1] = 1;
	triangle[0].vertex_indices[2] = 2;

	triangle[1].vertex_indices[0] = 2;
	triangle[1].vertex_indices[1] = 3;
	triangle[1].vertex_indices[2] = 0;

	triangle[0].edge_indices[0] = 0;
	triangle[0].edge_indices[1] = 1;
	triangle[0].edge_indices[2] = 4;

	triangle[1].edge_indices[0] = 2;
	triangle[1].edge_indices[1] = 3;
	triangle[1].edge_indices[2] = 5;

	glm_vec4_copytriangle->vertices = quad->vertices;
	triangle->edges = quad->edges;
	triangle->vertex_normals = quad->vertex_normals;

	triangle->edges[triangle[0].edge_indices[0]];
	triangle->edges[triangle[0].edge_indices[1]];
	triangle->edges[triangle[0].edge_indices[2]];

	triangle->edges[triangle[1].edge_indices[0]];
	triangle->edges[triangle[1].edge_indices[1]];
	triangle->edges[triangle[1].edge_indices[2]];

	return triangle;
}

vec3 triangle_indexed_normal(triangle_indexed_t* triangle)
{
	if(!triangle)
	{
		fprintf(stderr, "ERROR: triangle_indexed_normal(triangle) failed!");
		return NULL;
	}
	vec4 edge[6];
	glm_vec4_sub(triangle->vertices[triangle->vertex_indices[0]], triangle->vertices[triangle->vertex_indices[0]], edge[0]);
	glm_vec4_sub(triangle->vertices[triangle->vertex_indices[4]], triangle->vertices[triangle->vertex_indices[0]], edge[1]);
	glm_vec4_sub(triangle->vertices[triangle->vertex_indices[1]], triangle->vertices[triangle->vertex_indices[3]], edge[1]);
}

quad_t* quad_create(vec4 position, float width, float height)
{
	quad_t* quad = calloc(1, sizeof(quad_t));
	if(!quad)
	{
		fprintf(stderr, "ERROR: quad_create(...) failed!");
		return NULL;
	}
	/* bottom left */
	quad->vertices[0][0] = position[0] - width / 2.0f;
	quad->vertices[0][1] = position[1] - height / 2.0f;
	quad->vertices[0][2] = position[2];
	quad->vertices[0][3] = position[3];
	/* bottom right */
	quad->vertices[1][0] = position[0] + width / 2.0f;
	quad->vertices[1][1] = position[1] - height / 2.0f;
	quad->vertices[1][2] = position[2];
	quad->vertices[1][3] = position[3];
	/* top right */
	quad->vertices[2][0] = position[0] + width / 2.0f;
	quad->vertices[2][1] = position[1] + height / 2.0f;
	quad->vertices[2][2] = position[2];
	quad->vertices[2][3] = position[3];
	/* top left */
	quad->vertices[3][0] = position[0] - width / 2.0f;
	quad->vertices[3][1] = position[1] + height / 2.0f;
	quad->vertices[3][2] = position[2];
	quad->vertices[3][3] = position[3];

	glm_vec4_sub(quad->vertices[1], quad->vertices[0], quad->edges[0]);
	glm_vec4_sub(quad->vertices[2], quad->vertices[1], quad->edges[1]);
	glm_vec4_sub(quad->vertices[3], quad->vertices[2], quad->edges[2]);
	glm_vec4_sub(quad->vertices[0], quad->vertices[3], quad->edges[3]);

	/* diagonal triangle edges */
	glm_vec4_sub(quad->vertices[2], quad->vertices[0], quad->edges[4]);
	glm_vec4_sub(quad->edges[0], position, v); 
	glm_vec4_sub(quad->vertices[3], quad->vertices[1], quad->edges[5]);

	quad->triangles = triangle_indexed_create(quad);

	vec4 edge_a, edge_b;
	/* trianges[0] */
	glm_vec4_sub(quad->edges[0], position, edge_a);
	glm_vec4_sub(quad->edges[4], position, edge_b);
	glmc_vec3_crossn(edge_a, edge_b, quad->triangles[0]->face_normal);

	/* triangles[1] */
	glm_vec4_sub(quad->edges[1], position, edge_a);
	glm_vec4_sub(quad->edges[5], position, edge_b);
	glmc_vec3_crossn(edge_a, edge_b, quad->triangles[1]->face_normal);

	return quad;
}

typedef struct box_s
{
	union {
		quad_t faces[6];
		struct {
			quad_t front;
			quad_t back;
			quad_t top;
			quad_t bottom;
			quad_t left;
			quad_t right;
		};
	};

	vec3 vertex_nomals[24];
	vec3 face_normals[6];
	float radius[6];
	versor orientation;
	vec4 center;
	vec3 up;
} box_t;

typedef struct sphere_s
{
	versor orientation;
	vec4 center;
	vec3 up;
	float radius;
	float circumference;
} sphere_t;

typedef struct cylinder_s
{
	vec4 position;
	float radius;
	float circumference;
	float height;
	versor orientation;
	disc_t top_disc;
	disc_t bottom_disc;
	box_t* bounding_box;
	sphere_t* bounding_sphere;
} cylinder_t;

inline float circle_radius(float circumference)
{
	return circumference / (2.0f * M_PI);
}

inline float circle_circumference(float radius)
{
	return 2.0f * M_PI * radius;
}

box_t* box_create(vec4 position, float min_width, float min_height, float min_depth)
{
	box_t* box = calloc(1, sizeof(box_t));
	if(!box)
	{
		fprintf(stderr, "ERROR: create_box(vec4) { calloc(1, sizeof(box_t)) } failed!");
		return NULL;
	}

	glm_vec4_copy(position, box->center);
	box->radius[0] = min_depth / 2.0f;
	box->radius[1] = min_depth / 2.0f;
	box->radius[2] = min_width / 2.0f;
	box->radius[3] = min_width / 2.0f;
	box->radius[4] = min_height / 2.0f;
	box->radius[5] = min_height / 2.0f;

	vec4 v;
	glm_vec4_zero(v);
	v[3] = 1.0f;
	/* create front face */
	v[2] = position[2] + box->radius[0];
	box->front = quad_create(v, min_width, min_height);
	/* create back face */
	v[2] = position[2] - box->radius[1];
	box->back = quad_create(v, min_width, min_height);

	v[2] = 0.0f;

	/* create left face */
	v[0] = position[0] - box->radius[2];
	box->left = quad_create(v, min_width, min_height);
	/* create right face */
	v[0] = position[0] + box->radius[3];
	box->right = quad_create(v, min_width, min_height);

	v[0] = 0.0f;

	/* create bottom face */
	v[1] = position[1] - box->radius[4];
	box->bottom = quad_create(v, min_width, min_height);
	/* create top face */
	v[1] = position[1] + box->radius[5];
	box->top = quad_create(v, min_width, min_height);

	return box;
}

sphere_t* sphere_create(vec4 position, float radius)
{
	sphere_t* sphere = calloc(1, sizeof(sphere_t));

	if(!sphere)
	{
		fprintf(stderr, "ERROR: sphere_create(vec4) { calloc(1, sizeof(sphere_t)) } failed!");
		return NULL;
	}

	glm_vec4_copy(position, sphere->center);
	sphere->radius = radius;
	sphere->circumference = circle_circumference(sphere->radius);
}

cylinder_t* cylinder_create(float radius, float height, vec4 position)
{
	cylinder_t* cylinder = calloc(1, sizeof(cylinder_t));

	if(!cylinder)
		return NULL;

	glm_vec4_copy(cylinder->position, position);
	cylinder->height = height;
	cylinder->radius = radius;
	cylinder->top_disc.radius = radius;
	cylinder->bottom_disc.radius = radius;
	cylinder->circumference = circle_circumference(radius);
	cylinder->top_disc.circumference = cylinder->circumference;
	cylinder->bottom_disc.circumference = cylinder->circumference;

	vec4 p;
	glm_vec4_copy(position, p);
	p[1] += height / 2.0f;
	glm_vec4_copy(position, cylinder->top_disc.position);
	p[1] -= height;
	glm_vec4_copy(position, cylinder->bottom_disc.position);

	vec3 a,b;

	a[0] = radius - cylinder->top_disc.position[0];
	a[1] = 0.0f - cylinder->top_disc.position[1];
	a[2] = 0.0f - cylinder->top_disc.position[2];

	glmc_vec3_ortho(a, b);
	glmc_vec3_crossn(a,b, cylinder->top_disc.face_normal);

	a[0] = radius - cylinder->bottom_disc.position[0];
	a[1] = 0.0f - cylinder->bottom_disc.position[1];
	a[2] = 0.0f - cylinder->bottom_disc.position[2];

	glmc_vec3_ortho(a, b);
	glmc_vec3_crossn(a, b, cylinder->bottom_disc.face_normal);
	glmc_vec3_negate(cylinder->bottom_disc.face_normal);

	glm_quat_forp(position, cylinder->top_disc.face_normal, cylinder->top_disc.face_normal, cylinder->orientation);
	glm_quat_normalize(cylinder->orientation);

	cylinder->bounding_box = box_create(position, min_width, min_height, min_depth);
	cylinder->bounding_sphere = sphere_create(position, radius);

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
