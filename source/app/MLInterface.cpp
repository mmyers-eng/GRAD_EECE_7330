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
#include "MLInterface.hpp"

#include <math.h>
#include <string.h>
#include "tensorflow/lite/core/c/common.h"
#include "tensorflow/lite/micro/examples/hello_world/models/hello_world_float_model_data.h"
#include "tensorflow/lite/micro/examples/hello_world/models/hello_world_int8_model_data.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_profiler.h"
#include "tensorflow/lite/micro/recording_micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

constexpr int kTensorArenaSize = 4096;
alignas(16) uint8_t g_tensor_arena[kTensorArenaSize];
using OpResolver = tflite::MicroMutableOpResolver<1>;

struct FrameworkHandle
{
	const tflite::Model * model_ptr; //pointer to the tensorflow model
	std::unique_ptr<tflite::MicroInterpreter> interp_ptr;
	OpResolver op_resolver;
};

int MLInterface::RegisterOps()
{
	fw_ptr->op_resolver.AddFullyConnected();
	return 1;
}

MLInterface::MLInterface() : fw_ptr(std::make_unique<struct FrameworkHandle>())
{

}

void MLInterface::init(void)
{
	tflite::InitializeTarget();
	memset(&g_tensor_arena[0], 0xce, sizeof(g_tensor_arena));
	fw_ptr->model_ptr =::tflite::GetModel(g_hello_world_float_model_data);
	TFLITE_CHECK_EQ(fw_ptr->model_ptr->version(), TFLITE_SCHEMA_VERSION);
	RegisterOps();
	fw_ptr->interp_ptr = std::make_unique<tflite::MicroInterpreter>(fw_ptr->model_ptr,
									fw_ptr->op_resolver,
									g_tensor_arena,
									sizeof(g_tensor_arena));
	fw_ptr->interp_ptr->AllocateTensors();
}

float MLInterface::doInference(float input)
{
	fw_ptr->interp_ptr->input(0)->data.f[0] = input;
	fw_ptr->interp_ptr->Invoke();
	return(fw_ptr->interp_ptr->output(0)->data.f[0]);
}

MLInterface::~MLInterface()
{
}
