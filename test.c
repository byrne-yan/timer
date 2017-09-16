#include "js_timer.h"
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
//#include <pthread.h>
static bool done = false;
void timer_handler(union sigval val){
	static int count = 3;
	--count;
	pid_t tid = syscall(SYS_gettid);;
	printf("timer(0x%lx) event handled! %d threadid:0x%lx\n",*(long*)val.sival_ptr,count,(long)tid);

	if(count<1){
		js_timer_t id  = *(js_timer_t*)val.sival_ptr;

		if(0==js_timer_stop(id))
		{
			printf("timer stopped!\n");
		}else{
			printf("error:%d\n",errno);
		}
		done = true;
	}
}

int main(int argc, char* argv[])
{
	printf("pid:0x%lx\n",(long)getpid());
	js_timer_t timer1 = 0;
	while(!done){
		printf("bala bala bala...\r");
		if(!timer1)
		{
		if(js_timer_create(&timer1,timer_handler)==0)
		{
			printf("Timer created! 0x%lx\n",(long)timer1);
			if(0==js_timer_start(timer1,3,0))
			{
				printf("Timer started!\n");
			}
		}
		}
	}
}
