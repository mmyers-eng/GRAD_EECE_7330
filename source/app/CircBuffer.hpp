#pragma once

#include <vector>
#include <algorithm>
template <typename T, size_t N>
class CircBuffer
{
public:
	/**
	 * @brief constructor
	 * 
	 * @details initializes and zeros buffer
	 * 
	 */
	CircBuffer() 
	{
		reset();
	}
	/**
	 * @brief push
	 * 
	 * @details pushes a new value into the circular buffer
	 * 
	 * @param[in] val the value to push 
	 * @return true onlyif the push was sucessful
	 */
	bool push(const T& val)
	{
		bool rtn{false};
		if(!isFull)
		{
			buffer[write] = val;
			write = (write + 1) % N;
			itemsInBuff++;
			if(write == read)
			{
				isFull = true;	
			}
			rtn = true;
		}
		return(rtn);
	}

	/**
	 * @brief pop
	 * 
	 * @details pops a value off of the circ buff
	 * 
	 * @param[out] val destination of the value to pop
	 * @return true onlyif a value was popped
	 */
	bool pop(T& val)
	{
		bool rtn{false};
		if(itemsInBuff > 0)
		{
			val = buffer[read];
			read = (read + 1) % N;
			itemsInBuff--;
			isFull = false;
			rtn = true;
		}
		return(rtn);
	}

	/**
	 * @brief peek
	 * 
	 * @details look at the first item in the buffer without removing it
	 * 
	 * @param[out] val destination of the peek value 
	 * @return true onlyif a value was read
	 */
	bool peek(T& val)
	{
		bool rtn{false};
		if (itemsInBuff > 0)
		{
			val = buffer[read];
			rtn = true;
		}
		return(rtn);
	}
	
	/**
	 * @brief reset
	 * 
	 * @details resets and re-initializes the entire buffer
	 * 
	 * @return void 
	 */
	void reset()
	{
		read = 0;
		write = 0;
		itemsInBuff = 0;
		isFull = false;
		buffer.fill(T{});
	}
		
private:

	int read; ///current buffer read index
	int write; ///current buffer write index
	int itemsInBuff; ///number of items in the buffer
	bool isFull; ///determines if the buffer is full or not.
	std::array<T, N> buffer; ///array holding actual buffer elements.
};
 
