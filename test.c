#include "js_timer.h"
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>

#ifdef __linux__
	#include <unistd.h>
	#include <sys/syscall.h>
#elif defined(_WIN32)
	#include <windows.h>
#endif // __linux__

//#include <pthread.h>
static int done = 0;
void timer_handler(js_timer_value val){
	static int count = 3;
	--count;
#ifdef __linux__
	pid_t tid = syscall(SYS_gettid);
#elif defined(_WIN32)
	DWORD tid = GetCurrentThreadId();
#endif // __linux__
	printf("timer(0x%lx) event handled! %d threadid:0x%lx\n",*(long*)val.sival_ptr,count,(long)tid);

	if(count<1){
		js_timer_t* pid  = (js_timer_t*)val.sival_ptr;

		if(0==js_timer_stop(*pid))
		{
			printf("timer stopped!\n");
		}else{
			printf("error:%d\n",errno);
		}
		done = 1;
	}
}

int main(int argc, char* argv[])
{
#ifdef __linux__
	printf("pid:0x%lx\n",(long)getpid());
#elif defined(_WIN32)
	printf("pid:0x%lx\n", (long)GetCurrentThreadId());
#endif // __linux__
	js_timer_t timer1;
	int timerCreated = 0;

	while(!done){
		printf("bala bala bala...\r");
		if(!timerCreated && js_timer_create(&timer1,timer_handler)==0)
		{
			timerCreated = 1;
			printf("Timer created! 0x%lx\n",*(long*)&timer1);
			if(0==js_timer_start(timer1,3,0))
			{
				printf("Timer started!\n");
			}
		}
#if defined(_WIN32)
		SleepEx(0,  TRUE);       // Put thread in an alertable state
#endif
	}
}
