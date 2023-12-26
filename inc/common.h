#ifndef COMMON_H
#define COMMON_H

#if !TEST
#define F_CPU 8000000
#include <util/delay.h>
#else
void _delay_ms(int32_t ms);
#endif



#endif
