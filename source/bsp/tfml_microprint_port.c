#include <stdio.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

//MJM NOTE: This entire file exists because of an issue
//with tflite-micro/tensorflow/lite/micro/micro_log.cc L40
//Symptom: vprintf won't call __write and write characters
//to console.
//which appears to cause a corruption issue by messing with the
//va_list. It can be removed if this issue gets resolved
void MicroPrintf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}

void VMicroPrintf(const char* format, va_list args)
{
    vfprintf(stderr, format, args);
}

void DebugLog(const char* format, va_list args)
{
    vfprintf(stderr, format, args);
}

#ifdef __cplusplus
}
#endif
