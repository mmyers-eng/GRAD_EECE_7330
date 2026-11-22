#pragma once
#include <memory>
#include <cstdint>
struct FrameworkHandle;
class MLInterface
{

public:
	MLInterface();
	~MLInterface();
	void init(void);
	int doInference(int8_t * input, size_t bufferSize);
	int getOutput(int8_t * output, size_t bufferSize);
	void printOutput(void);
private:
	static constexpr uint32_t inputSize_bytes{3072};
	static constexpr uint32_t outputSize_bytes{10};
	
	int RegisterOps();
	std::unique_ptr<struct FrameworkHandle> fw_ptr;
	int8_t m_output[outputSize_bytes];
			
};
