#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include "driver_uart.h"

int _write(int file, char *ptr, int len)
{
    int rtn = -1;
    // Check if the stream is stdout (1) or stderr (2)
    if (file == STDOUT_FILENO || file == STDERR_FILENO)
    {
	    for(int idx = 0; idx < len; idx++)
	    {
		    uart_print_char(ptr[idx]);
	    }
	    rtn = len;
    }
    return rtn;
}


void _exit(int status) {
    // Print the exit code so you see why TFLM or Newlib aborted
    printf("\r\n*** SYSTEM ABORT / EXIT CALLED (status code: %d) ***\r\n", status);
    
    // Disable interrupts to freeze system state
    __asm__ volatile ("cpsid i");

    // Halt GDB automatically if a debugger is attached
    __asm__ volatile ("bkpt #0");

    // Fallback infinite loop
    while (1) {
        // Keep CPU in low-power sleep while halted
        __asm__ volatile ("wfi");
    }
}
