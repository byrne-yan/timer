#ifndef _JS_TIMER_H
#define _JS_TIMER_H
#include <time.h>
#include <signal.h>

#ifdef __linux__
	typedef timer_t js_timer_t;
	typedef sigval js_timer_value;

#endif
typedef void (*js_timer_handler)(js_timer_value val);

int js_timer_create(js_timer_t* timerid, js_timer_handler handler);
int js_timer_start(js_timer_t timerid,long sec, long nsec);
int js_timer_stop(js_timer_t timerid);
int js_timer_delete(js_timer_t timerid);

int js_timer_settime(js_timer_t timerid, struct itimerspec* new_value, struct itimerspec* old_value);

#endif 
