#include <cstdint>
#include <cstdio>
#include "MLInterface.hpp"
#include "SerialUtil.hpp"
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


void AppSine(void)
{
	printf("Init \r\n");
	MLInterface ml;
	SerialUtil su;
	ml.init();
	Parser rxParser{};
	handle = &rxParser;
	while(true)
	{
		PacketFrame pkt;
		if(handle->getPacket(pkt))
		{
			auto payload = pkt.getPayload();
			auto result = su.toFloat(payload.data(),payload.size());
			std::vector<uint8_t> outBuff(sizeof(float), 0xff);
			if(result.first)
			{
				float mlOut = ml.doInference(result.second);
				su.fromFloat(mlOut, outBuff.data(), outBuff.size());
			}
			for(auto c : outBuff)
			{
				uart_print_char(c);
			}
		}
	}
}
