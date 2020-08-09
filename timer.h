#ifndef _TIMER_H
#define _TIMER_H

#include "system_types.h"

typedef struct time_system_s
{
	double t_now;
	double t_last;
	double dt;
	double fps;
	uint64_t tick_now;
	uint64_t tick_last;
	uint64_t freq;
} time_system_t;

time_system_t* time_system_create();
double time_system_update(time_system_t* timer);
bool time_system_halt(time_system_t* timer);

#endif
