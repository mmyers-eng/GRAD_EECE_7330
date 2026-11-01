#include "driver_uart.h"
#include "stm32f405xx.h"
#include "driver_cpu.h"

void init_bsp()
{
	__disable_irq();
	enable_fpu();
	uart_init();
	__enable_irq();
}
