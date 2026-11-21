#include <cstdint>
#include <cstdio>
//#include "MLInterface.hpp"
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
extern "C" int main() {
	init_bsp();
	printf("Init \r\n");
	MLInterface ml;
	SerialUtil su;
	ml.init();
	//initialize the parser
	Parser rxParser{};
	handle = &rxParser;
	printf("Test Start \r\n");
	//printf("%f\r\n", ml.doInference());
	ml.doInference(cifar_9779_deer_int8_hwc);

// 	ml.doInference(&cifar_9779_deer_int8_hwc[0], sizeof(cifar_9779_deer_int8_hwc));
// //TEST CODE	
// 	//Uncomment for debug print
// 	const char* kCategoryLabels[10] = {
// 		"airplane",
// 		"automobile",
// 		"bird",
// 		"cat",
// 		"deer",
// 		"dog",
// 		"frog",
// 		"horse",
// 		"ship"
// 		"truck",
// 	};
// 	std::vector<int8_t>outTemp(10, 0xff);
// 	ml.getOutput(outTemp.data(), outTemp.size());
	
// 	for(uint32_t i = 0; i < outTemp.size(); i++)
// 	{
// 	 	printf("categorty %s, prob %f \n\r",
// 		       kCategoryLabels[i],
// 		       static_cast<float>((outTemp[i] + 128)) * 0.00390625);
// 	}

// //END TEST	
	printf("Test Done \r\n");
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
	return 0;

#endif
	
#if 0
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
	return 0;
#endif
}
