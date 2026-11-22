#include <cstdint>
#include <cstdio>
#include "ImageClassML.hpp"
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

extern signed char cifar_9779_deer_int8_hwc[3072];

void AppImg(void)
{
	printf("Init \r\n");
	MLInterface ml;
	SerialUtil su;
	ml.init();
	Parser rxParser{};
	handle = &rxParser;
	ml.doInference(&cifar_9779_deer_int8_hwc[0], sizeof(cifar_9779_deer_int8_hwc));
#if 0	
	while(true)
	{
		PacketFrame pkt;
		if(handle->getPacket(pkt))
		{
			auto payload = pkt.getPayload();
			std::vector<int8_t>outBuff(10, 0xff);
			if(payload.size() == 3072)
			{
				ml.doInference(reinterpret_cast<int8_t *>(payload.data()), payload.size());
				ml.getOutput(outBuff.data(), outBuff.size());
			}
			for(auto c : outBuff)
			{
				uart_print_char(c);
			}
		}
	}
#endif
}
