#include "util.h"
#include "../hal/usart.h"

#if !TEST // avoid errors with cmock
static FILE mystdout = FDEV_SETUP_STREAM(Util_PutChar_Printf, NULL, _FDEV_SETUP_WRITE);

/**
 * Util_Init() - Initialize printf
 *
 * Return: void
 */
void Util_Init(void)
{
    stdout = &mystdout;
}
#else
void Util_Init(void)
{
    
}
#endif

/**
 * Util_PutChar_Printf()
 * @byte: byte to pass to output to printf
 * @stream: 
 *
 * Credit to "https://efundies.com/avr-and-printf/" on how to link printf.
 * This function is called by printf as a stream handler.
 * Return: 
 */
int Util_PutChar_Printf(char byte, FILE *stream) {
    // translate \n to \r for br@y++ terminal
    if (byte == '\n') {
	Usart_Tx('\r');
    }
    Usart_Tx(byte);
  
    return 0;
}
