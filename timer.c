#include "timer.h"

time_system_t* time_system_create()
{
	time_system_t* timer = calloc(1, sizeof(time_system_t));
	timer->t_now = glfwGetTime();
	timer->t_last = 0;
	timer->dt = 0;
	timer->fps = 0;
	timer->tick_now = glfwGetTimerValue();
	timer->tick_last = 0;
	timer->freq = glfwGetTimerFrequency();
	return timer;
}


double time_system_update(time_system_t* timer)
{
	timer->t_last = timer->t_now;
	timer->t_now = glfwGetTime();
	timer->dt = timer->t_now - timer->t_last;
	timer->fps = 1.0 / timer->dt;
	timer->tick_last = timer->tick_now;
	timer->tick_now = glfwGetTimerValue();
	timer->freq = glfwGetTimerFrequency();
	return timer->dt;
}

bool time_system_halt(time_system_t* timer)
{
	if(!timer)
		return false;
	free(timer);
	return true;
}
