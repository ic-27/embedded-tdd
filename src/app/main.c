#define F_CPU 8000000

#include <stdio.h>
#include "executor.h"

#if TEST
int MAIN(void)
#else
    int main(void)
#endif
{
    Executor_Init();
    printf("Hello world, it is lovely to use printf in AVR!\n");
    printf("Bye bye, cruel world...\n");
    
    while (1) {
	
    }
}


