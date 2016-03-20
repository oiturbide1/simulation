#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define THREAD_NUM 1
#ifdef _WIN32
#include <windows.h>
#endif
#ifdef _WIN64
#include <windows.h>
#endif
#ifdef __linux__
#include <unistd.h>
#endif

typedef struct clock{
    int counter;
    int cycles;
    int sleepTime;
}clockType;


//Maybe pass completed processes list
//And if it reaches the max number of processes
//End the clock


int threadSleep(int sec){
    #ifdef __linux__
        sleep(sec);
        return 1;
    #endif
    #ifdef _WIN64
        Sleep(sec);
        return 1;
    #elif _WIN32
        Sleep(sec);
        return 1;
    #endif // _WIN32
    return 0;
}

void *clockFunc(void *args){
    int i, nap, iter;
    clockType * clk;
    clk = (clockType*)args;
    clk->counter = 0;
    #ifdef _WIN32
    nap = clk->sleepTime * 1000;
    #endif // WINDOWS
    iter = clk->cycles;
    for (i = 0; i < iter; ++i){
        threadSleep(nap);
        ++(clk->counter);
        printf("Current Clock Time is:%d\n", clk->counter);
    }
    pthread_exit(NULL);
    return 0;
}


int main()
{
    clockType *clk;
    pthread_t threads[THREAD_NUM];
    int rc,i;
    clk = malloc(sizeof(clockType));
    clk->cycles = 10; //Total number of clock spins
    clk->sleepTime = 1; //1 Second
    rc = pthread_create(&threads[0],NULL,clockFunc,(clockType*) clk);
    if(rc){
        printf("ERROR. Return code from thread %d\n", rc);
        exit(-1);
    }

    for(i = 0; i < THREAD_NUM; ++i)
	{
	   rc = pthread_join(threads[i], NULL);
	   if(rc)
	   {
		   printf("Error, return code from thread_join() is %d\n", rc);
		   exit(-1);
	   }
	   printf("main completed join with thread %d\n", i);
	}
    return 0;
}
