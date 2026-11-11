#pragma once
#include <memory>
struct FrameworkHandle;
class MLInterface
{

public:
	MLInterface();
	~MLInterface();
	void init(void);
	float doInference(float input);
private:	
	int RegisterOps();
	std::unique_ptr<struct FrameworkHandle> fw_ptr;
};
