#pragma once
#include "Head.h"


inline int timeToBlocks(float time, float srate, float blockSize, float timeDivisor = 1000.0f)
{
	// second to blocks
	// time = unit time, and time divisor converts to true time.
	// set time divisor to 1000 for milliseconds, or 1 for seconds 
	// srate (samples/sec) * seconds / blockSize (samples/block) + 1;
	// gives a minimum of 1 block when seconds = 0
	return srate * (time/timeDivisor) / blockSize + 1;
}

fvec weightRatio(const fvec& arr, int octSize);

void weightRatio(fvec& ratios, const fvec& arr, int octSize);

