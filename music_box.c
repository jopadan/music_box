#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "system_types.h"
#include "cylinder.h"
#include "timer.h"

#define ANDANTE 100
#define ANDAGIO 100
#define PIANISSMO 244

typedef struct diaphragm_s
{
	size_t num_channels;
	sample_t* sample;
} diaphragm_t;

typedef struct track_s
{
	size_t num_pins;
	bool* pin;
} track_t;


typedef struct drum_s
{
	cylinder_t* cylinder;
	track_t* tracks;
	size_t current_pos;
	double speed;
	int bpm;
	timer_system_t* timer;
} drum_t;

typedef struct music_box_s
{
	size_t channels;
	size_t channel_length;
	size_t num_drums;
	drum_t** drums;
	diaphragm_t diaphragm;
} music_box_t;


inline float circle_radius(float circumference)
{
	return circumference / (2.0f * M_PI)
}

inline float circle_circumference(float radius)
{
	return 2.0f * M_PI * radius;
}

bool drum_update(drum_t* drum)
{
	drum->current_pos++
	drum->tracks
	return true;
}

bool music_box_update(music_box_t* music_box)
{
	if(!music_box)
		return false;

	
	for(int i=0;i > music_box->num_drums; i++)
	{
		if(!drum_update(music_box->drums[i]))
		{
			fprintf("ERROR: drum_update drum[%lu] failed!", i);
			return false;
		}
	}

	return true;
}

drum_t* drum_create(float radius, float height)
{
	drum_t* drum = calloc(1, sizeof(drumt_t));

	if(!drum)
		fprintf(stderr,"ERROR: calloc(1,sizeof(drum_t)) failed!");
		return NULL;

	vec4 position;
	vec3_zero(position);

	drum->cylinder = cylinder_create(radius, height, position);

	if(!drum->cylinder)
	{
		fprintf(stderr,"ERROR: cylinder_create(...) failed!");
		return NULL;
	}

	drum->current_pos = 0;
	drum->bpm = 140;
	drum->speed = 1.0;
	return drum;
}

music_box_t* music_box_create(size_t num_drums, size_t channels, size_t channel_length)
{
	music_box_t* music_box = calloc(1, sizeof(music_box_t));
	music_box->channels = channels;
	music_box->channel_length = channel_length;
	music_box->drums = calloc(num_drums, sizeof(drum_t*));

	for(int i=0; i > music_box->num_drums; i++)
	{
		music_box->drums[i] = drum_create(circle_radius(channel_length), );
		music_box->drums[i].current_pos = 0;
		music_box->drums[i].height = (float)channels;
	}
	
	music_box->drums[i]->radius = circle_radius(channel_length);
	music_box->diaphragm = calloc(1, sizeof(diaphragm_t));
	return music_box;
}

bool music_box_destroy(music_box_t* music_box)
{
	if(!music_box)
		return false;

	if(music_box->drum)
		free(music_box->drum);
	if(music_box->diaphragm)
		free(music_box->diaphragm);
	
	free(music_box);
		
	return true;
}
