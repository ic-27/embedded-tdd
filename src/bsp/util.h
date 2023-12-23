#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

void Util_Init(void);
#if !TEST // avoid errors with cmock. fine since we ONLY use for printf anyway
int Util_PutChar_Printf(char var, FILE *stream);
#endif

#endif // UTIL_H
