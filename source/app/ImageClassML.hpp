#pragma once
#include <memory>
struct FrameworkHandle;
class MLInterface
{

public:
	MLInterface();
	~MLInterface();
	void init(void);
	int doInference(int8_t input[3072]);
private:	
	int RegisterOps();
	std::unique_ptr<struct FrameworkHandle> fw_ptr;
};
