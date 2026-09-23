#include <cstdint>
#include <cstdio>
#include "tfml_test.hpp"
#include "driver_bsp.h"

extern int hello_world_test(int argc, char* argv[]);
extern "C" int main() {
	init_bsp();
	printf("test of printf\n\r");
	run_tfml_test();
	while (true) {
		// Main loop
	}
	return 0;
}
