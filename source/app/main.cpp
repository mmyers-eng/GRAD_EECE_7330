#include <cstdint>
#include <cstdio>
#include "driver_bsp.h"

extern void AppImg(void);
//extern void AppSine(void);

extern "C" int main() {
	init_bsp();
	AppImg();
//	AppSine();
	printf("Test Done \r\n");
}
