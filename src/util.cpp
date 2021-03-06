#include <stdint.h>

#include "util.hpp"

void* memcpy(void* destination, const void* source, size_t num)
{
	if(num > 0)
	{
		const uint8_t* src = (const uint8_t*) source;
		uint8_t* dest = (uint8_t*) destination;
		for(size_t i = 0; i < num; i++)
		{
			dest[i] = src[i];
		}
	}
	return destination;
}

/*
 * There are four distinct cases here:
 * 	1. destination and source blocks do not overlap
 * 	2. destination and source are the same
 * 	3. destination and source do overlap; destination starts before source
 * 	4. destination and source do overlap; destination starts after source
 *
 * Memcpy results in expected behavior in all cases except for case 4. In that
 * case, copying must be done backwards to avoid reading from bytes that have
 * already been overwritten.
 */
void* memmove(void* destination, const void* source, size_t num)
{
	if(num > 0)
	{
		if(((size_t) destination) < ((size_t) source) || ((size_t) destination) >= (((size_t) source)) + num)
		{
			return memcpy(destination, source, num);
		}
		else if(destination != source)
		{
			size_t i = num - 1;
			const uint8_t* src = (const uint8_t*) source;
			uint8_t* dest = (uint8_t*) destination;
			do
			{
				dest[i] = src[i];
				i--;
			} while(i != 0);
		}
	}
	return destination;
}

int memcmp(const void* ptr1, const void* ptr2, size_t num)
{
	const uint8_t* a = (const uint8_t*) ptr1;
	const uint8_t* b = (const uint8_t*) ptr2;
	for(size_t i = 0; i < num; i++)
	{
		if(a[i] < b[i])
			return -1;
		else if(a[i] > b[i])
			return 1;
	}
	return 0;
}

void* memset (void* ptr, int value, size_t num)
{
	uint8_t* dest = (uint8_t*) ptr;
	uint8_t v = (uint8_t) value;
	for(size_t i = 0; i < num; i++)
	{
		dest[i] = v;
	}
	return ptr;
}

void __cxa_pure_virtual()
{
    // do nothing
}