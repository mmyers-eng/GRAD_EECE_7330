.syntax unified
.cpu cortex-m4
.thumb

.global g_pfnVectors
.global Reset_Handler

/* Set top of stack to end of internal SRAM (128KB on STM32F4) */
.equ STACK_TOP, 0x20020000

.section .isr_vector,"a",%progbits
.type g_pfnVectors, %object
g_pfnVectors:
    .word STACK_TOP
    .word Reset_Handler

.section .text.Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
    /* Call C++ main function */
    bl main
    b .
