#include <cstdint>
#include <cstdio>
#include "tfml_test.hpp"
#include "driver_bsp.h"
#include "Parser.hpp"
#include "driver_uart.h"
extern "C" {
extern void uart_loop_back_test(void);
}

static Parser * handle = NULL;
extern "C"
{
//caution this function is called in an interrupt.	
void uart_process_rx(const uint8_t rx_byte)
{
	if(handle != NULL)
	{
		//TODO race conditions WILL happen here
		handle->parseByte(rx_byte);
	}
}

}

extern "C" int main() {
	init_bsp();
	//initialize the parser
	Parser rxParser{};
	handle = &rxParser;
	run_tfml_test();
	while(true)
	{
		PacketFrame pkt;
		if(handle->getPacket(pkt))
		{
			auto payload = pkt.getPayload();
			for(auto c : payload)
			{
				uart_print_char(c);
			}
		}
	}
	return 0;
}
