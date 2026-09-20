#include <cstdint>
#include <cstdio>
#include "driver_bsp.h"

extern int hello_world_test(int argc, char* argv[]);
extern "C" int main() {
	init_bsp();
	printf("test of printf\n\r");
	hello_world_test(0, 0);
	while (true) {
		// Main loop
	}
	return 0;
}
