#pragma once

#include "Head.h"

template <class T>
class Buffer
{
public:
    const int size;
    std::vector<T> vec;
    int head = 0;

    Buffer(int size, T fill) : size(size), vec(size, fill)
    {
    };

    
    std::vector<T> toOrderedVec()
    {
        std::vector<T> output(size, 0);

        for (int i = 0; i < size; i++)
        {
            output[i] = vec[(i + head) % size];
        }

        return output;
    };

    void push(T val)
    {
        vec[head] = val;
        //head = (head + 1) % size;
        //inline ternary generally 2-3 times as fast as modulo on repl.it
        head = head + 1 < size ? head + 1 : 0;
    };

    T lastValue()
    {
        if (head == 0) { return vec[size - 1]; }

        return vec[head - 1];
    };

    T currentValue()
    {
        return vec[head];
    }

    T& operator[](int i)
    {
        int ind = head + i >= size ? head + i - size : head + i;
        return vec[ind];
    }
    
    
};

/*
int main() {
  int loopsize = 100000;
  int size = 2000;
  int head = 0;

  auto start = std::chrono::steady_clock::now();

  for(int i = 0; i < loopsize; i++)
  {
    head = head + 1< size ? head + 1 : 0;
  }

  auto end = std::chrono::steady_clock::now();

  auto condTime = end - start;

  start = std::chrono::steady_clock::now();
  head = 0;

  for(int i = 0; i < loopsize; i++)
  {
    head = (head + 1) % size;
  }

  end = std::chrono::steady_clock::now();

  auto modTime = end - start;

  std::cout << "Conditional time: "
  << std::chrono::duration_cast<std::chrono::microseconds>(condTime).count() << "\n"
  << "Modulo time: "
  << std::chrono::duration_cast<std::chrono::microseconds>(modTime).count() << "\n";
}
*/