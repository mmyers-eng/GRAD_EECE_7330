#include "SerialUtil.hpp"
#include <cstring>
std::pair<bool, float>  SerialUtil::toFloat(const uint8_t * buff, size_t buffSize)
{
	std::pair<bool, float> rtn{false, 0.0f};
	if(buff != nullptr)
	{
		if(sizeof(float) == buffSize)
		{
			memcpy(&(rtn.second), buff, sizeof(float));
			rtn.first=true;
		}
	}
	return(rtn);
}

bool SerialUtil::fromFloat(float val, uint8_t * dstBuff, size_t buffSize)
{
	bool rtn{false};
	if(dstBuff != nullptr)
	{
		if(buffSize >= sizeof(val))
		{
			memcpy(dstBuff, &val, sizeof(val));
			rtn=true;
		}
	}
	return(rtn);
}
