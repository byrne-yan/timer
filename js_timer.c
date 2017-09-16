#include "js_timer.h"

#ifdef _WIN32
#define _SECOND 10000000

VOID CALLBACK win32_timer_handler(
	LPVOID lpArgToCompletionRoutine,
	DWORD dwTimerLowValue,
	DWORD dwTimerHighValue
)
{
	handler_args* args = (handler_args*)lpArgToCompletionRoutine;
	if (args && args->handler)
	{
		args->handler(args->value);
	}
}

#endif
int js_timer_create(js_timer_t* timerid, js_timer_handler handler)
{
#ifdef __linux__
	struct sigevent sigev;
	sigev.sigev_notify = SIGEV_THREAD;
	sigev.sigev_notify_function = handler;
	sigev.sigev_value.sival_ptr = timerid;
	return timer_create(CLOCK_REALTIME, &sigev,timerid);
#elif defined(_WIN32)
	HANDLE hTimer = NULL;

	// Create an unnamed waitable timer.
	hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
	if (NULL == hTimer)
	{
		return -1;
	}

	timerid->handle = hTimer;
	timerid->args = malloc(sizeof(handler_args));
	timerid->args->handler = handler;
	timerid->args->value.sival_ptr = timerid;
	return 0;
#endif
}

#ifdef __linux__

int js_timer_settime(js_timer_t timerid, struct itimerspec* new_value, struct itimerspec* old_value)
{
	return timer_settime(timerid,0,new_value,old_value);
}
#endif // __linux__


int js_timer_start(js_timer_t timerid, long sec, long nsec)
{
#ifdef __linux__
	struct itimerspec spec;
	spec.it_interval.tv_sec = sec;
	spec.it_interval.tv_nsec = nsec;
	spec.it_value.tv_sec = sec;
	spec.it_value.tv_nsec = nsec;
	return js_timer_settime(timerid, &spec, NULL);
#elif defined(_WIN32)
	LARGE_INTEGER due;
	//due.QuadPart = -(sec*10000000+nsec/100);
	due.QuadPart = -1* _SECOND;
	//BOOL res = SetWaitableTimer(timerid.handle,&due ,sec*1000+nsec/1000 , win32_timer_handler , &timerid, TRUE);
	BOOL res = SetWaitableTimer(timerid.handle, &due, 1000, win32_timer_handler, timerid.args, FALSE);
	return !res? 0 : -1;
#endif
}
int js_timer_stop(js_timer_t timerid)
{
#ifdef __linux__
	struct itimerspec spec = {0,0};
	return js_timer_settime(timerid,&spec,NULL);

#elif defined(_WIN32)
	BOOL res = CancelWaitableTimer(timerid.handle);
	return res == TRUE ? 0 : -1;
#endif
}

int js_timer_delete(js_timer_t timerid)
{
#ifdef __linux__
	return timer_delete(timerid);
#elif defined(_WIN32)
	BOOL res = CloseHandle(timerid.handle);
	if (res) {
		free(timerid.args);
		return 0;
	}
	return -1;
#endif
}
