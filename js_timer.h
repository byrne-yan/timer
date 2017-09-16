#ifndef _JS_TIMER_H
#define _JS_TIMER_H
#include <time.h>
#include <signal.h>

#if defined(_WIN32)
	#include <windows.h>
#endif // __linux__

#ifdef __linux__
	typedef timer_t js_timer_t;
	typedef sigval js_timer_value;
#elif _WIN32
	typedef union js_timer_value {
		int sival_int;
		void* sival_ptr;
	} js_timer_value;
#endif

typedef void(*js_timer_handler)(js_timer_value val);

#ifdef _WIN32
	typedef struct handler_args {
		js_timer_handler handler;
		js_timer_value value;
	}handler_args;

	typedef struct js_timer_t {
		HANDLE handle;
		handler_args* args;
	}js_timer_t;
#endif // _WIN32


int js_timer_create(js_timer_t* timerid, js_timer_handler handler);
int js_timer_start(js_timer_t timerid,long sec, long nsec);
int js_timer_stop(js_timer_t timerid);
int js_timer_delete(js_timer_t timerid);

#ifdef __linux__
int js_timer_settime(js_timer_t timerid, struct itimerspec* new_value, struct itimerspec* old_value);
#endif
#endif 
