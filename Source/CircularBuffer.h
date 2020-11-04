#pragma once

#include "Head.h"

template <class T>
struct Buffer
{
    Buffer(int size, T fill);
    const int size;
    std::vector<T> toOrderedVec();

    void push(T val);

    T lastValue();
    T currentValue();
    
    fvec vec;
    int head = 0;
};

