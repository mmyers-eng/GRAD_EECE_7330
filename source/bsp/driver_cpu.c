#include "stm32f405xx.h"

void enable_fpu(void)
{
    // Enable full access to Coprocessors CP10 and CP11 (FPU)
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));
    
    // ARM Assembly Data & Instruction Synchronization Barriers
    __asm__ volatile ("dsb" ::: "memory");
    __asm__ volatile ("isb" ::: "memory");
}
