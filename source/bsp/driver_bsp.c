#include "driver_uart.h"
#include "stm32f405xx.h"
void init_bsp()
{
	__disable_irq();
	uart_init();
	__enable_irq();
}
