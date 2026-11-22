/* Copyright 2023 The TensorFlow Authors. All Rights Reserved.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
   ==============================================================================*/
#include "ImageClassML.hpp"

#include <math.h>
#include <string.h>
#include "tensorflow/lite/core/c/common.h"
//#include "tensorflow/lite/micro/examples/hello_world/models/hello_world_float_model_data.h"
//#include "tensorflow/lite/micro/examples/hello_world/models/hello_world_int8_model_data.h"
#include "ic_model_quant_data.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_profiler.h"
#include "tensorflow/lite/micro/recording_micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

// (* 64 1024)
constexpr int kTensorArenaSize = 65536;
alignas(16) uint8_t g_tensor_arena[kTensorArenaSize];
using OpResolver = tflite::MicroMutableOpResolver<10>;

struct FrameworkHandle
{
	const tflite::Model * model_ptr; //pointer to the tensorflow model
	std::unique_ptr<tflite::MicroInterpreter> interp_ptr;
	OpResolver op_resolver;
};
//Required TFLM Ops: {'ADD', 'DELEGATE', 'SOFTMAX', 'AVERAGE_POOL_2D', 'RESHAPE', 'FULLY_CONNECTED', 'CONV_2D'}
int MLInterface::RegisterOps()
{
	fw_ptr->op_resolver.AddAdd();
	fw_ptr->op_resolver.AddAveragePool2D();
	fw_ptr->op_resolver.AddReshape();
	fw_ptr->op_resolver.AddFullyConnected();
	fw_ptr->op_resolver.AddConv2D();
	fw_ptr->op_resolver.AddSoftmax();
	return 1;
}

MLInterface::MLInterface() : fw_ptr(std::make_unique<struct FrameworkHandle>())
{

}

void MLInterface::init(void)
{
	tflite::InitializeTarget();
	memset(&g_tensor_arena[0], 0xce, sizeof(g_tensor_arena));
	fw_ptr->model_ptr =::tflite::GetModel(pretrainedResnet_quant_tflite);
	TFLITE_CHECK_EQ(fw_ptr->model_ptr->version(), TFLITE_SCHEMA_VERSION);
	RegisterOps();
	printf("making arena \n\r");
	fw_ptr->interp_ptr = std::make_unique<tflite::MicroInterpreter>(fw_ptr->model_ptr,
									fw_ptr->op_resolver,
									g_tensor_arena,
									sizeof(g_tensor_arena));
	printf("Allocate Tensors \n\r");
	fw_ptr->interp_ptr->AllocateTensors();
	printf("done allocate \n\r");
}

int MLInterface::doInference(int8_t * input, size_t bufferSize)
{

	printf("Do infer \n\r");
	if(inputSize_bytes == bufferSize)
	{
		
		memcpy(fw_ptr->interp_ptr->input(0)->data.int8, input, inputSize_bytes);
		printf("Invoke \n\r");
		fw_ptr->interp_ptr->Invoke();
		printOutput();
	}
	return(0);
}

void MLInterface::printOutput(void)
{
	const char* kCategoryLabels[10] = {
		"airplane",
		"automobile",
		"bird",
		"cat",
		"deer",
		"dog",
		"frog",
		"horse",
		"ship"
		"truck",
	};
	//Note output will be 1 x 10 int8 array with a
	//0.00390625 * (q + 128) scale
	constexpr uint32_t outSize{10};
	printf("Get Output \n\r");
	TfLiteTensor* output = fw_ptr->interp_ptr->output(0);
	float result{0.0};
	
	for(uint32_t i = 0; i < outSize; i++)
	{
		result = static_cast<float>((output->data.int8[i] + 128)) * 0.00390625;
		printf("categorty %s, prob %f \n\r", kCategoryLabels[i], result);
	}

}

MLInterface::~MLInterface()
{
}
