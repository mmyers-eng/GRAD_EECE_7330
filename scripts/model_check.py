import tensorflow as tf

#interpreter = tf.lite.Interpreter(model_path="../tflite-micro/tensorflow/lite/micro/examples/hello_world/models/hello_world_float.tflite")
interpreter = tf.lite.Interpreter(model_path="../source/models/image_class/model_info/pretrainedResnet_quant.tflite")
interpreter.allocate_tensors()

# Extract unique operator names
op_details = interpreter._get_ops_details()
ops = set(op['op_name'] for op in op_details)
print("Required TFLM Ops:", ops)
