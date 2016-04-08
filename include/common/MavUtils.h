#pragma once
#include <ctime>
#include <stdint.h>

inline uint64_t getTimeNanoSecond()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * (uint64_t)1000000000 + ts.tv_nsec;
} 
