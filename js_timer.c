#include "js_timer.h"


int js_timer_create(js_timer_t* timerid, js_timer_handler handler)
{
	struct sigevent sigev;
	sigev.sigev_notify = SIGEV_THREAD;
	sigev.sigev_notify_function = handler;
	sigev.sigev_value.sival_ptr = timerid;
	return timer_create(CLOCK_REALTIME, &sigev,timerid);
}

int js_timer_settime(js_timer_t timerid, struct itimerspec* new_value, struct itimerspec* old_value)
{
	return timer_settime(timerid,0,new_value,old_value);
}


int js_timer_start(js_timer_t timerid, long sec, long nsec)
{
	struct itimerspec spec;
	spec.it_interval.tv_sec = sec;
	spec.it_interval.tv_nsec = nsec;
	spec.it_value.tv_sec = sec;
	spec.it_value.tv_nsec = nsec;
	return js_timer_settime(timerid, &spec, NULL);
}
int js_timer_stop(js_timer_t timerid)
{
	
	struct itimerspec spec = {0,0};
	return js_timer_settime(timerid,&spec,NULL);
}

int js_timer_delete(js_timer_t timerid)
{
	return timer_delete(timerid);
}
