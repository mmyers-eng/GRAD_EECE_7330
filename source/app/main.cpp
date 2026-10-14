#include <cstdint>
#include <cstdio>
#include "tfml_test.hpp"
#include "driver_bsp.h"
extern "C" {
extern void uart_loop_back_test(void);
}
// Direct memory-mapped access to Cortex-M System Control Block CPACR register
//TODO Move this into bsp
#define SCB_CPACR_PTR ((volatile uint32_t*)0xE000ED88)
inline void enable_fpu() {
    // Enable full access to Coprocessors CP10 and CP11 (FPU)
    *SCB_CPACR_PTR |= ((3UL << 10 * 2) | (3UL << 11 * 2));
    
    // ARM Assembly Data & Instruction Synchronization Barriers
    __asm__ volatile ("dsb" ::: "memory");
    __asm__ volatile ("isb" ::: "memory");
}
extern "C" int main() {
	//TODO move to startup
	enable_fpu();
	init_bsp();
	run_tfml_test();
	while(true)
	{
	}
	return 0;
}
