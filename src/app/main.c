#include "main.h"
#include "executor.h"

#if TEST
#define MAIN TEST_MAIN
#else
#define MAIN main
#endif
int MAIN(void)
{
    Executor_Init();

    while (1) {
	Executor_Exec();
    }
}


