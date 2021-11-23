#pragma once
#ifndef CUSTOM_ALGORITHM
#define CUSTOM_ALGORITHM



#include<math.h>
#include<stdint.h>

static class AlgorithmUtil
{
public:
	// 是否是2的N次幂
	static bool IsPowerOf2(uint64_t x)
	{
		return (x > 0) && !(x & (x - 1));
	}

	static int GetLog2(uint64_t data)
	{
		//return int(log2(num)); C++11
		int n = -1;
		while (data)
		{
			data >>= 1;
			n++;
		}
		return n;
	}

	static bool NearEqual(float a, float b, float ep = 1e-06)
	{
		return abs(a - b) <= ep;
	}
};
#endif //CUSTOM_ALGORITHM