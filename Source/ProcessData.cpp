#include "ProcessData.h"


fvec weightRatio(const fvec& arr, int octSize)
{
    fvec output(arr.size(), 0);

    for(int i = octSize; i < arr.size(); i++)
    {
        output[i] = arr[i] /arr[i - octSize];
        output[i] = output[i] > 10 ? 10 : output[i];
    }

    return output;
}

void weightRatio(fvec& ratios, const fvec& arr, int octSize)
{

    for(int i = octSize; i < arr.size(); i++)
    {
        ratios[i] = arr[i] /arr[i - octSize];
        ratios[i] = ratios[i] > 10 ? 10 : ratios[i];
    }
}