#ifndef DRIVER_UART_H
#define DRIVER_UART_H

#ifdef __cplusplus
extern "C" {
#endif
	
void uart_init(void);
void uart_print_char(char c);

#ifdef __cplusplus
}
#endif

#endif //DRIVER_UART_H
