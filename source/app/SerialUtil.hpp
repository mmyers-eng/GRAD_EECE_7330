#pragma once
#include <utility>
#include <cstdint>
#include <cstddef>
class SerialUtil
{
public:
	SerialUtil(){};
	/**
	 * @brief toFloat
	 * 
	 * @details converts a byte buffer to a float
	 * 
	 * @param[in] buff pointer to a byte buffer to convert
	 * @param[in] buffSize 
	 * @return pair<bool, float:
	 *         first: true only if the conversion was successful
	 *         second: the resulting float value, 0.0 when failure occurs.x
	 */
	std::pair<bool, float> toFloat(const uint8_t * buff, size_t buffSize);

	/**
	 * @brief fromFloat
	 * 
	 * @details packs a float into a byte buffer
	 * 
	 * @param[in] val value to pack into the buffer
	 * @param[in] buff pointer to a byte buffer to move the value into
	 * @param[in] buffSize size of the buffer
	 * @return bool true only if the conversion was successful
	 */
	bool fromFloat(float val, uint8_t * dstBuff, size_t buffSize);

};
